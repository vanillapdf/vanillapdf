#ifndef _FIELD_TREE_H
#define _FIELD_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/fields.h"

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace vanillapdf {
namespace semantics {

// The field hierarchy of an interactive form (12.7.3) - a view over the
// root /Fields array, the same way PageTree is a view over the page tree
// root. The array is the top of the hierarchy: its entries are the
// root-level fields, and every field below is reached through /Kids. The
// tree knows nothing about the form holding the array; Create builds the
// array that InteractiveForm::SetFieldTree installs as the form's /Fields
// entry, so a read never creates anything and a tree is never a view over
// an object the document does not hold.
//
// Two views over one structure:
//  - the flat view (GetFieldCount / GetField / TryFindField) enumerates the
//    resolved terminal fields in document order, which is what form filling
//    consumers work with
//  - the structural view starts at the root level (GetRootChildCount /
//    GetRootChild) and continues down through Field::GetChildCount /
//    GetChild; Field::GetParent walks back up and stops at a root-level
//    field, which has no /Parent
//
// Every mutation goes through the tree, because the tree owns the cache.
// The raw dictionary API can still edit /Fields, /Kids or /Parent underneath;
// Invalidate is the documented contract for that path.
class FieldTree : public HighLevelObject<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>> {
public:
    explicit FieldTree(syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> fields);

    // Creates an empty hierarchy belonging to the document. It takes effect
    // once attached to a form with InteractiveForm::SetFieldTree. The
    // document is the public form, mirrored by the C API; the file overload
    // is for the library's own use, where Document holds its file directly.
    static FieldTreePtr Create(DocumentPtr document);
    static FieldTreePtr Create(syntax::FilePtr file);

    // Flat view - terminal fields in document order, grouping nodes hidden.
    // A radio button group is a single terminal field with one value,
    // regardless of how many widget annotations represent it on the page.
    types::size_type GetFieldCount() const;
    FieldPtr GetField(types::size_type index) const;

    // Finds a terminal field by its fully qualified name - the /T partial
    // names joined with '.' (12.7.3.2), UTF-8 encoded - as the hierarchy
    // walk produces it: the traversal parent's name extended by the node's
    // own /T, so the name index agrees with the enumeration by
    // construction. Field::GetQualifiedName has nothing but /Parent to
    // follow and can disagree on a file whose /Parent entries are wrong.
    // Duplicate names resolve to the first terminal field in document
    // order.
    bool TryFindField(std::string_view qualified_name, OuputFieldPtr& result) const;

    // Structural view, level 0 - the root /Fields entries that are field
    // dictionaries, in array order, groups included; a stray dictionary is
    // skipped with a warning, the same way a widget among /Kids is. Served
    // from the cache under the lock, like the flat view.
    // Field::GetChildCount / GetChild continue below.
    types::size_type GetRootChildCount() const;
    FieldPtr GetRootChild(types::size_type index) const;

    // Adds a root-level field: appends its reference to /Fields and removes
    // any /Parent it carries, since Table 220 requires /Parent for kids
    // only. InsertRootChild places it among the /Fields entries instead; an
    // index equal to the count appends, a larger one is an
    // ObjectMissingException.
    //
    // The child may bring a subtree of its own; every node of it is held
    // to the same rules as the child, under the names the walk will give
    // them once the child is in place.
    //
    // InvalidParameterException when: the child dictionary is a direct
    // object (the arrays hold indirect references, Table 218 and Table 220),
    // the child or a node of its subtree is already in this tree, or a
    // fully qualified name the subtree would take is already taken - by
    // this tree or by another node of the same subtree.
    void AddRootChild(FieldPtr child);
    void InsertRootChild(types::size_type index, FieldPtr child);

    // Adds a child field under a parent of this tree: appends its reference
    // to the parent's /Kids, created when the parent has none yet, and sets
    // /Parent. InsertChild places it among the /Kids entries instead, with
    // the same index rules as InsertRootChild.
    //
    // Refused as for AddRootChild, and in addition when the parent does not
    // belong to this tree, or is a terminal field carrying widget
    // annotations or merged with its own widget annotation.
    //
    // The order of /Fields and /Kids affects enumeration order only - tab
    // order comes from the page /Annots.
    void AddChild(FieldPtr parent, FieldPtr child);
    void InsertChild(FieldPtr parent, types::size_type index, FieldPtr child);

    // Removes a field of this tree - root-level or nested, subtree included
    // - from the container the hierarchy walk reached it through, and clears
    // its /Parent. Emptied groups are left in place, and widget annotations
    // stay in the page /Annots. A container listing the same node twice
    // loses the first entry only.
    //
    // InvalidParameterException when the field does not belong to this
    // tree, or when the container no longer holds it - the hierarchy was
    // edited underneath the tree without Invalidate.
    void RemoveChild(FieldPtr field);

    // Drops the cache, so the next access rebuilds it from the
    // dictionaries. Required after editing /Fields, /Kids or /Parent through
    // the raw dictionary API; the tree's own mutators maintain themselves.
    void Invalidate();

private:
    // The cache has two tiers, so that authoring many fields stays linear:
    //  - the index - every node the walk visited, keyed by object identity
    //    and mapped to its traversal parent (the node whose /Kids reached
    //    it, empty for a root-level entry), plus the qualified names of the
    //    nodes carrying a /T - answers membership and uniqueness. The insert
    //    mutators extend it with the subtree they validated, no rebuild.
    //  - the views - the root children, the terminal fields in document
    //    order and the name lookup - are ordered, so an insert only marks
    //    them stale; the next read rebuilds everything in one walk.
    // Removal and Invalidate drop both tiers.
    //
    // The traversal parent is what the mutators navigate and name by;
    // /Parent is validated and warned about during the walk, never trusted,
    // since a missing or mismatched entry is the common way a file is
    // already dirty. The walk threads the name prefix down with it, so the
    // name index agrees with the enumeration by construction. The index
    // holds the nodes alive, so an address can never be recycled by an
    // unrelated dictionary while it is a key.
    //
    // The mutators hold the cache lock across the whole mutation, so the
    // membership check, the create-if-missing /Kids insert and the
    // reference append are atomic with respect to concurrent mutators and
    // cache builds on this tree instance. Distinct FieldTree wrappers over
    // the same array each carry their own cache and lock (canonicalization
    // is issue #524).
    using NodeMap = std::unordered_map<syntax::DictionaryObjectPtr, syntax::OutputDictionaryObjectPtr, DeferredIdentityHash<syntax::DictionaryObject>, DeferredIdentityEqual<syntax::DictionaryObject>>;

    mutable std::unique_ptr<std::recursive_mutex> m_cache_lock;
    mutable NodeMap m_nodes;
    mutable std::set<std::string> m_names;
    mutable bool m_index_built = false;

    mutable std::vector<syntax::DictionaryObjectPtr> m_root_children;
    mutable std::vector<syntax::DictionaryObjectPtr> m_field_cache;
    mutable std::map<std::string, syntax::DictionaryObjectPtr> m_terminal_index;
    mutable bool m_views_built = false;

    void EnsureIndexBuilt() const;
    void EnsureViewsBuilt() const;
    void BuildFieldCache() const;

    // The traversal parent is the node whose /Kids reached this one; empty
    // for a root entry, which the /Fields array reached. Its qualified name
    // is the prefix of every name below it - the root level has none.
    void BuildFieldCacheInternal(
        syntax::IndirectReferenceObjectPtr node_reference,
        const syntax::OutputDictionaryObjectPtr& traversal_parent,
        const std::string& parent_name,
        std::map<syntax::IndirectReferenceId, bool>& visited) const;

    bool IsMember(const syntax::DictionaryObjectPtr& dictionary) const;

    // The walk's fully qualified name of a member node, derived from the
    // chain of traversal parents in the node index - the same chain the
    // mutators navigate by, not /Parent
    std::string GetMemberQualifiedName(const syntax::DictionaryObjectPtr& dictionary) const;

    // The /Kids array of a node, created when the node has none yet
    static syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> CreateKids(syntax::DictionaryObjectPtr parent);

    // The insertion rules shared by every level; an empty parent stands for
    // the root level. Collects the child's subtree as the walk will see it
    // under that parent - the nodes with their traversal parents and the
    // names they take - ready to extend the index once the child is in
    // place.
    void ValidateInsertion(
        const syntax::OutputDictionaryObjectPtr& parent,
        const FieldPtr& child,
        NodeMap& subtree_nodes,
        std::set<std::string>& subtree_names) const;

    // The rules every node of an inserted subtree is held to, under the
    // name the walk will give it: not a member yet, and its name - when it
    // has a /T - taken neither by this tree nor by another node of the
    // subtree. Recurses through the /Kids fields the way the walk does.
    void ValidateSubtree(
        const syntax::DictionaryObjectPtr& node,
        const syntax::OutputDictionaryObjectPtr& traversal_parent,
        const std::string& parent_name,
        NodeMap& subtree_nodes,
        std::set<std::string>& subtree_names) const;

    // Adds a validated subtree to the index and marks the views stale
    void ExtendIndex(const NodeMap& subtree_nodes, const std::set<std::string>& subtree_names) const;
    void LinkChild(const syntax::OutputDictionaryObjectPtr& parent, syntax::DictionaryObjectPtr child) const;
};

} // semantics
} // vanillapdf

#endif /* _FIELD_TREE_H */
