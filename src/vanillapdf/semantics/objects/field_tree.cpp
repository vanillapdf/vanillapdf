#include "precompiled.h"

#include "semantics/objects/field_tree.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/document.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

#include "utils/buffer.h"
#include "utils/text_string_encoding.h"

namespace vanillapdf {
namespace semantics {

// The walk's fully qualified name of a node: the traversal parent's name
// extended by the node's own /T. A node without a /T contributes no
// segment and shares its parent's name (12.7.3.2).
static std::string QualifiedNameOf(const std::string& parent_name, const syntax::DictionaryObjectPtr& node) {
    if (!node->Contains(constant::Name::T)) {
        return parent_name;
    }

    auto partial_name = node->FindAs<syntax::StringObjectPtr>(constant::Name::T);

    // /T is a text string (7.9.2.2) - normalizing every segment to UTF-8
    // lets PDFDocEncoding and UTF-16BE partial names join into a single
    // coherent name
    auto partial_name_utf8 = TextStringEncoding::ToUtf8(partial_name->GetValue()->ToStringView());
    if (parent_name.empty()) {
        return partial_name_utf8;
    }

    return parent_name + "." + partial_name_utf8;
}

FieldTree::FieldTree(syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> fields) : HighLevelObject(fields) {
    m_cache_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

FieldTreePtr FieldTree::Create(DocumentPtr document) {
    return Create(document->GetFile());
}

FieldTreePtr FieldTree::Create(syntax::FilePtr file) {

    // The array itself is a direct object of the form dictionary, but it
    // belongs to the file, so that the references it will hold resolve
    syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> fields;
    auto fields_data = fields->Data();
    fields_data->SetFile(file);
    fields_data->SetInitialized();

    return make_deferred<FieldTree>(fields);
}

// Flat view

types::size_type FieldTree::GetFieldCount() const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();
    return m_field_cache.size();
}

FieldPtr FieldTree::GetField(types::size_type index) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();

    if (index >= m_field_cache.size()) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Field index out of range: {}", index);
    }

    return Field::Create(m_field_cache[index]);
}

bool FieldTree::TryFindField(std::string_view qualified_name, OuputFieldPtr& result) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();

    auto found = m_terminal_index.find(std::string(qualified_name));
    if (found == m_terminal_index.end()) {
        return false;
    }

    result = Field::Create(found->second);
    return true;
}

// Structure

types::size_type FieldTree::GetRootChildCount() const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();
    return m_root_children.size();
}

FieldPtr FieldTree::GetRootChild(types::size_type index) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();

    if (index >= m_root_children.size()) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Root child index out of range: {}", index);
    }

    return Field::Create(m_root_children[index]);
}

void FieldTree::AddRootChild(FieldPtr child) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    syntax::OutputDictionaryObjectPtr root_level;
    ValidateInsertion(root_level, child);

    auto child_dictionary = child->GetObject();
    auto child_reference = make_deferred<syntax::IndirectReferenceObject>(child_dictionary);
    _obj->Append(child_reference);

    LinkChild(root_level, child_dictionary);
    Invalidate();
}

void FieldTree::InsertRootChild(types::size_type index, FieldPtr child) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    syntax::OutputDictionaryObjectPtr root_level;
    ValidateInsertion(root_level, child);

    // An index equal to the size appends, the array refuses anything beyond
    if (index > _obj->GetSize()) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Root child index out of range: {}", index);
    }

    auto child_dictionary = child->GetObject();
    auto child_reference = make_deferred<syntax::IndirectReferenceObject>(child_dictionary);
    _obj->Insert(index, child_reference);

    LinkChild(root_level, child_dictionary);
    Invalidate();
}

void FieldTree::AddChild(FieldPtr parent, FieldPtr child) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    syntax::OutputDictionaryObjectPtr parent_dictionary(parent->GetObject());
    ValidateInsertion(parent_dictionary, child);

    auto child_dictionary = child->GetObject();
    auto child_reference = make_deferred<syntax::IndirectReferenceObject>(child_dictionary);

    auto kids = CreateKids(*parent_dictionary);
    kids->Append(child_reference);

    LinkChild(parent_dictionary, child_dictionary);
    Invalidate();
}

void FieldTree::InsertChild(FieldPtr parent, types::size_type index, FieldPtr child) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    syntax::OutputDictionaryObjectPtr parent_dictionary(parent->GetObject());
    ValidateInsertion(parent_dictionary, child);

    auto kids = CreateKids(*parent_dictionary);

    // An index equal to the size appends, the array refuses anything beyond
    if (index > kids->GetSize()) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Child index out of range: {}", index);
    }

    auto child_dictionary = child->GetObject();
    auto child_reference = make_deferred<syntax::IndirectReferenceObject>(child_dictionary);
    kids->Insert(index, child_reference);

    LinkChild(parent_dictionary, child_dictionary);
    Invalidate();
}

void FieldTree::RemoveChild(FieldPtr field) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    EnsureCacheBuilt();

    auto field_dictionary = field->GetObject();
    auto node = m_nodes.find(field_dictionary);
    if (node == m_nodes.end()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field does not belong to this field hierarchy");
    }

    // The container is the one the hierarchy walk reached the field
    // through - the root /Fields array, or the /Kids of the traversal
    // parent. /Parent is not consulted: a missing one would send a nested
    // field to the root array, a wrong one to another node's /Kids.
    auto traversal_parent = node->second;

    syntax::MixedArrayObjectPtr container = _obj->Data();
    if (!traversal_parent.empty()) {
        container = traversal_parent->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
    }

    // Only a reference can be this node - the walk skips every other kind
    // of entry, so none of them ever became a member
    bool removed = false;
    auto container_size = container->GetSize();
    for (decltype(container_size) i = 0; i < container_size; ++i) {
        syntax::ObjectPtr entry = container->GetValue(i);
        if (!syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(entry)) {
            continue;
        }

        auto entry_reference = syntax::ObjectUtils::ConvertTo<syntax::IndirectReferenceObjectPtr>(entry);
        if (!entry_reference->GetReferencedObject()->Identity(field_dictionary)) {
            continue;
        }

        removed = container->Remove(i);
        break;
    }

    // The index knows the field but the container does not hold it: the
    // hierarchy was edited underneath the tree, and the index describes a
    // structure that no longer exists
    if (!removed) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field is no longer held by the container the hierarchy walk reached it through - the hierarchy was edited underneath the tree, call Invalidate first");
    }

    if (field_dictionary->Contains(constant::Name::Parent)) {
        bool parent_removed = field_dictionary->Remove(constant::Name::Parent);
        assert(parent_removed && "Unable to remove existing item"); UNUSED(parent_removed);
    }

    Invalidate();
}

void FieldTree::Invalidate() {
    ACCESS_LOCK_GUARD(m_cache_lock);

    m_root_children.clear();
    m_field_cache.clear();
    m_nodes.clear();
    m_terminal_index.clear();
    m_names.clear();
    m_cache_built = false;
}

// Cache

void FieldTree::EnsureCacheBuilt() const {
    if (!m_cache_built) {
        BuildFieldCache();
    }
}

void FieldTree::BuildFieldCache() const {

    // Both /Fields and /Kids shall contain indirect references (Table 218,
    // Table 220). The arrays are walked untyped and every entry classified
    // by Field::ClassifyChildEntry - the same classification the field's
    // own child walk applies - so a malformed entry is skipped with a
    // warning here exactly where it is skipped there, and every node is
    // dereferenced only after the cycle check.
    //
    // Malformed documents can link /Kids in a cycle. Visited nodes are
    // tracked by their object identity, the same way Field guards its
    // /Parent chain walk.
    std::map<syntax::IndirectReferenceId, bool> visited;
    syntax::OutputDictionaryObjectPtr root_level;

    for (auto entry : _obj->Data()) {
        syntax::OutputDictionaryObjectPtr root_child;
        auto entry_type = Field::ClassifyChildEntry(entry, root_child);
        if (entry_type == Field::ChildEntryType::Malformed) {
            continue;
        }

        auto field_reference = syntax::ObjectUtils::ConvertTo<syntax::IndirectReferenceObjectPtr>(entry);

        // The root array holds fields only (Table 218) - widget
        // annotations belong to a field's /Kids. A dictionary here that is
        // not a field is a stray entry, skipped the same way
        if (entry_type != Field::ChildEntryType::Field) {
            spdlog::warn("Root /Fields entry {} {} R is not a field dictionary and is skipped", field_reference->GetReferencedObjectNumber(), field_reference->GetReferencedGenerationNumber());
            continue;
        }

        // A node listed twice - or already reached through some /Kids - is
        // enumerated where it was first reached; the root view shows what
        // the walk shows
        if (m_nodes.find(*root_child) != m_nodes.end()) {
            spdlog::warn("Root /Fields entry {} {} R was already reached - only the first path is enumerated", field_reference->GetReferencedObjectNumber(), field_reference->GetReferencedGenerationNumber());
            continue;
        }

        m_root_children.push_back(*root_child);
        BuildFieldCacheInternal(field_reference, root_level, std::string(), visited);
    }

    m_cache_built = true;
}

void FieldTree::BuildFieldCacheInternal(
    syntax::IndirectReferenceObjectPtr node_reference,
    const syntax::OutputDictionaryObjectPtr& traversal_parent,
    const std::string& parent_name,
    std::map<syntax::IndirectReferenceId, bool>& visited) const {

    auto object_number = node_reference->GetReferencedObjectNumber();
    auto generation_number = node_reference->GetReferencedGenerationNumber();
    syntax::IndirectReferenceId node_id(object_number, generation_number);

    // A node reached a second time is a cycle when it is an ancestor and a
    // node shared by two parents otherwise; either way it is one node and
    // is enumerated where it was first reached
    auto found = visited.find(node_id);
    if (found != visited.end() && found->second) {
        spdlog::warn("Field {} {} R is reached by more than one /Fields or /Kids entry - a cycle or a node shared by two parents; only the first path is enumerated", object_number, generation_number);
        return;
    }

    visited[node_id] = true;

    auto node = node_reference->GetReferencedObjectAs<syntax::DictionaryObjectPtr>();
    m_nodes.emplace(node, traversal_parent);

    // A /Parent that disagrees with the /Kids entry that reached the node
    // is the other common way a file is already dirty. Reported, not
    // repaired: the walk is the authority for the name and the container,
    // and Field::GetQualifiedName, which follows /Parent, diverges from
    // here on.
    bool parent_is_root = traversal_parent.empty();
    if (node->Contains(constant::Name::Parent)) {
        auto parent_obj = node->Find(constant::Name::Parent);
        bool parent_matches = false;
        if (!parent_is_root && syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(parent_obj)) {
            auto parent_reference = syntax::ObjectUtils::ConvertTo<syntax::IndirectReferenceObjectPtr>(parent_obj);
            parent_matches = traversal_parent->Identity(parent_reference->GetReferencedObject());
        }

        if (!parent_matches) {
            spdlog::warn("Field {} {} R has a /Parent entry that does not reference the node whose /Kids contains it", object_number, generation_number);
        }
    } else if (!parent_is_root) {
        spdlog::warn("Field {} {} R is a /Kids entry without the /Parent entry required by Table 220", object_number, generation_number);
    }

    // Names are indexed for the nodes that carry a /T of their own - a node
    // without one shares its parent's fully qualified name (12.7.3.2) and
    // would only ever report a false duplicate
    bool terminal = Field::IsTerminalDictionary(node);
    auto name = QualifiedNameOf(parent_name, node);
    if (node->Contains(constant::Name::T)) {
        auto inserted = m_names.insert(name);
        if (!inserted.second) {
            spdlog::warn("Duplicate fully qualified field name \"{}\" - lookups resolve to the first field in document order", name);
        }

        if (terminal) {
            m_terminal_index.emplace(name, node);
        }
    }

    if (terminal) {
        m_field_cache.push_back(node);
        return;
    }

    // Only field dictionaries are hierarchy nodes - widget annotations stay
    // attached to their field (12.7.3.2)
    auto kids = node->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
    for (auto entry : kids) {
        syntax::OutputDictionaryObjectPtr kid;
        if (Field::ClassifyChildEntry(entry, kid) != Field::ChildEntryType::Field) {
            continue;
        }

        auto kid_reference = syntax::ObjectUtils::ConvertTo<syntax::IndirectReferenceObjectPtr>(entry);
        BuildFieldCacheInternal(kid_reference, syntax::OutputDictionaryObjectPtr(node), name, visited);
    }
}

// Helpers

bool FieldTree::IsMember(const syntax::DictionaryObjectPtr& dictionary) const {
    EnsureCacheBuilt();
    return m_nodes.find(dictionary) != m_nodes.end();
}

std::string FieldTree::GetMemberQualifiedName(const syntax::DictionaryObjectPtr& dictionary) const {
    auto node = m_nodes.find(dictionary);
    assert(node != m_nodes.end() && "The caller checks membership first");

    // The chain of traversal parents ends at the root level, which has no
    // name; it cannot cycle, since the walk visits every node once
    std::string parent_name;
    if (!node->second.empty()) {
        parent_name = GetMemberQualifiedName(*node->second);
    }

    return QualifiedNameOf(parent_name, dictionary);
}

syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> FieldTree::CreateKids(syntax::DictionaryObjectPtr parent) {
    if (!parent->Contains(constant::Name::Kids)) {
        syntax::MixedArrayObjectPtr mixed_array;
        mixed_array->SetFile(parent->GetFile());
        mixed_array->SetInitialized();

        parent->Insert(constant::Name::Kids, mixed_array);
    }

    return parent->FindAs<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>>(constant::Name::Kids);
}

void FieldTree::ValidateInsertion(const syntax::OutputDictionaryObjectPtr& parent, const FieldPtr& child) const {
    auto child_dictionary = child->GetObject();

    // The container arrays hold indirect references (Table 218, Table 220)
    // - a direct dictionary cannot be referenced and would serialize as a
    // dangling 0 0 R
    if (!child_dictionary->IsIndirect()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field dictionary shall be an indirect object - allocate a cross-reference entry for it first");
    }

    EnsureCacheBuilt();

    std::string parent_name;

    if (!parent.empty()) {
        auto parent_dictionary = *parent;

        // Nothing but the node index tells a parent of this tree from a
        // field of another document - a field is a plain dictionary view
        if (!IsMember(parent_dictionary)) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "The parent field does not belong to this field hierarchy");
        }

        // A field's /Kids holds either child fields or widget annotations,
        // never both (12.7.3.1)
        if (parent_dictionary->Contains(constant::Name::Kids) && Field::IsTerminalDictionary(parent_dictionary)) {
            auto parent_kids = parent_dictionary->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
            if (parent_kids->GetSize() > 0) {
                LOG_ERROR_AND_THROW(InvalidParameterException, "A terminal field carrying widget annotations cannot take child fields");
            }
        }

        // A field merged with its widget annotation is a leaf of the page
        // as well as of the hierarchy - giving it children would make the
        // widget dictionary a grouping node
        if (parent_dictionary->Contains(constant::Name::Subtype)) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "A field merged with its widget annotation cannot take child fields");
        }

        parent_name = GetMemberQualifiedName(parent_dictionary);
    }

    // The child may bring a subtree of its own - every node of it becomes
    // a node of this tree and is held to the same rules, under the name
    // the walk will give it once the child is in place
    std::set<std::string> subtree_names;
    std::unordered_set<syntax::DictionaryObjectPtr, DeferredIdentityHash<syntax::DictionaryObject>, DeferredIdentityEqual<syntax::DictionaryObject>> subtree_nodes;
    ValidateSubtree(child_dictionary, parent_name, subtree_names, subtree_nodes);
}

void FieldTree::ValidateSubtree(
    const syntax::DictionaryObjectPtr& node,
    const std::string& parent_name,
    std::set<std::string>& subtree_names,
    std::unordered_set<syntax::DictionaryObjectPtr, DeferredIdentityHash<syntax::DictionaryObject>, DeferredIdentityEqual<syntax::DictionaryObject>>& subtree_nodes) const {

    // A node reached twice within the subtree is enumerated once by the
    // walk, so it is judged once here
    if (!subtree_nodes.insert(node).second) {
        return;
    }

    if (m_nodes.find(node) != m_nodes.end()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field {} {} R already belongs to this field hierarchy", node->GetObjectNumber(), node->GetGenerationNumber());
    }

    // Fully qualified names shall be unique (12.7.3.2) - within this tree
    // and within the subtree itself; a node without a /T has no name of its
    // own to collide with
    auto qualified_name = QualifiedNameOf(parent_name, node);
    if (node->Contains(constant::Name::T)) {
        if (m_names.find(qualified_name) != m_names.end()) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "A field with the fully qualified name \"{}\" already exists in this field hierarchy", qualified_name);
        }

        if (!subtree_names.insert(qualified_name).second) {
            LOG_ERROR_AND_THROW(InvalidParameterException, "The fully qualified name \"{}\" is taken by more than one field of the subtree being added", qualified_name);
        }
    }

    if (Field::IsTerminalDictionary(node)) {
        return;
    }

    auto kids = node->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
    for (auto entry : kids) {
        syntax::OutputDictionaryObjectPtr kid;
        if (Field::ClassifyChildEntry(entry, kid) != Field::ChildEntryType::Field) {
            continue;
        }

        ValidateSubtree(*kid, qualified_name, subtree_names, subtree_nodes);
    }
}

void FieldTree::LinkChild(const syntax::OutputDictionaryObjectPtr& parent, syntax::DictionaryObjectPtr child) const {

    // Root-level fields have no /Parent (Table 220 requires it for kids
    // only); anything below the root references its parent
    if (parent.empty()) {
        if (child->Contains(constant::Name::Parent)) {
            bool removed = child->Remove(constant::Name::Parent);
            assert(removed && "Unable to remove existing item"); UNUSED(removed);
        }

        return;
    }

    auto parent_reference = make_deferred<syntax::IndirectReferenceObject>(*parent);
    child->Insert(constant::Name::Parent, parent_reference, true);
}

} // semantics
} // vanillapdf
