#ifndef _C_FIELD_TREE_H
#define _C_FIELD_TREE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_field_tree.h
    * \brief This file contains class definitions for \ref FieldTreeHandle
    */

    /**
    * \class FieldTreeHandle
    * \extends IUnknownHandle
    * \ingroup group_fields
    * \brief
    * The field hierarchy of an interactive form (PDF 1.2, section 12.7.3).
    *
    * The hierarchy is a tree whose top is the root /Fields array: its
    * entries are the top-level fields, and a non-terminal field groups
    * other fields through /Kids for naming and attribute inheritance. The
    * tree is the view over that array, the same way \ref PageTreeHandle is
    * the view over the page tree root.
    *
    * The tree offers two views over the hierarchy, told apart by their
    * vocabulary - *Field* is the flat view, *Child* the structure:
    *
    * - The flat view - \ref FieldTree_GetFieldCount, \ref FieldTree_GetField
    *   and \ref FieldTree_FindField - enumerates the resolved terminal
    *   fields in document order. Terminal fields are the logical fields a
    *   user interacts with; the grouping nodes are hidden, the same way the
    *   page tree hides its interior nodes. This is the default path for
    *   form filling.
    * - The structural view starts at the top level with
    *   \ref FieldTree_GetRootChildCount and \ref FieldTree_GetRootChild and
    *   continues down through \ref Field_GetChildCount and
    *   \ref Field_GetChild; \ref Field_GetParent walks back up and stops at
    *   a top-level field, which has no /Parent.
    *
    * Every field mutation goes through the tree, because the tree owns the
    * cache behind the flat view: \ref FieldTree_AddRootChild and
    * \ref FieldTree_InsertRootChild at the top level, \ref FieldTree_AddChild
    * and \ref FieldTree_InsertChild below it, \ref FieldTree_RemoveChild at
    * either. Editing /Fields, /Kids or /Parent through the dictionary API
    * underneath is possible; call \ref FieldTree_Invalidate afterwards.
    *
    * Obtain the tree of an existing form from
    * \ref InteractiveForm_GetFieldTree; give a form that has none yet an
    * empty one with \ref FieldTree_CreateFromDocument and
    * \ref InteractiveForm_SetFieldTree.
    *
    * Thread safety: every FieldTree_* call on one tree handle is
    * serialized on the tree's own lock, so the flat view, the top level
    * and the mutators may be used concurrently on the same tree - a
    * mutation is atomic and a concurrent enumeration observes it entirely
    * or not at all. The Field_* calls are not part of that: a field is a
    * plain view over its dictionary, so \ref Field_GetChild,
    * \ref Field_GetParent and the field getters and setters run outside
    * the tree's lock, as does any edit through the dictionary API. Walking
    * the structure through Field_GetChild while another thread mutates the
    * tree is a data race on the /Kids arrays, the same as any other
    * unsynchronized dictionary access.
    *
    * For more details please visit [section 12.7.3 - Field Dictionaries](PDF32000_2008.pdf#G11.2110737).
    */

    /**
    * \memberof FieldTreeHandle
    * @{
    */

    /**
    * \brief
    * Create an empty field hierarchy belonging to the document.
    *
    * The hierarchy is fully usable right away - fields can be added to it
    * before it is attached - but it does not take effect until it is
    * attached to a form with \ref InteractiveForm_SetFieldTree, which
    * installs this very array as the form's /Fields entry. Creating and
    * attaching are deliberately separate steps, so that reading a document
    * never modifies it.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_CreateFromDocument(DocumentHandle* handle, FieldTreeHandle** result);

    /**
    * \brief
    * Get the number of resolved terminal fields in the hierarchy.
    *
    * A radio button group is a single terminal field with one value,
    * regardless of how many widget annotations represent it on the page.
    * A group without any child field yet is indistinguishable from a field
    * without widgets and is enumerated as a terminal field until it receives
    * a child.
    *
    * Both views classify the /Fields and /Kids entries the same way, so
    * this is exactly the set of terminals the structural walk reaches. The
    * arrays shall hold indirect references (Table 218, Table 220); an entry
    * that is not one - a direct dictionary, say - does occur in existing
    * files and is skipped with a warning rather than failing the
    * enumeration, the same as a cyclic /Kids link.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetFieldCount(FieldTreeHandle* handle, size_type* result);

    /**
    * \brief
    * Get the resolved terminal field at the given zero-based index, in
    * document order.
    *
    * Indices are stable until the hierarchy is mutated. This index space is
    * the flat view's own - it is not a position for the structural
    * mutators.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the index is out
    * of range.
    * \see \ref FieldTree_GetFieldCount
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetField(FieldTreeHandle* handle, size_type index, FieldHandle** result);

    /**
    * \brief
    * Find a terminal field by its fully qualified name - the partial field
    * names (/T entries) joined with '.', UTF-8 encoded - the same form
    * \ref Field_GetQualifiedName produces, so its data can be passed back
    * here unchanged. The name is given with an explicit size rather than
    * as a NUL-terminated string because a partial name is a text string
    * that may legitimately contain U+0000, which UTF-8 encodes as a NUL
    * byte.
    *
    * The names are the ones the hierarchy walk produces - each field's
    * partial name appended to the name of the node whose /Kids reached it
    * - so they agree with the enumeration by construction.
    * Field_GetQualifiedName has nothing but the field's /Parent entries to
    * follow; in a well-formed file the two are the same, in a file whose
    * /Parent entries are missing or wrong they differ, and the walk's name
    * is the one to look up.
    *
    * Fully qualified names shall be unique (12.7.3.2), but existing documents
    * do not always honor that. Duplicates are tolerated when reading - a
    * warning is logged when the hierarchy is first enumerated, and the lookup
    * resolves to the first terminal field in document order - and rejected
    * when writing through the structural mutators.
    *
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when no terminal field has
    * that name.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_FindField(FieldTreeHandle* handle, string_type qualified_name, size_type size, FieldHandle** result);

    /**
    * \brief
    * Get the number of top-level fields - the entries of the root /Fields
    * array that are field dictionaries, groups included.
    *
    * The root array holds fields only (Table 218); a stray dictionary in it
    * - a widget annotation, say - is not a field and is skipped with a
    * warning, the same way a widget among a field's /Kids is skipped by
    * \ref Field_GetChildCount. This is the first level of the structural
    * view; Field_GetChildCount continues below it.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetRootChildCount(FieldTreeHandle* handle, size_type* result);

    /**
    * \brief
    * Get the top-level field at the given zero-based index, in /Fields
    * order.
    *
    * Together with \ref Field_GetChild this walks the whole hierarchy: the
    * top-level fields from here, every level below through the field.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the index is out
    * of range.
    * \see \ref FieldTree_GetRootChildCount
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_GetRootChild(FieldTreeHandle* handle, size_type index, FieldHandle** result);

    /**
    * \brief
    * Append a top-level field to the root /Fields array.
    *
    * Any /Parent entry the child carries is removed - Table 220 requires
    * /Parent for /Kids entries only. The container arrays hold indirect
    * references, so the child's underlying dictionary shall be registered
    * as an indirect object within the document, for example through
    * \ref File_AllocateNewEntry.
    *
    * The child may bring a subtree of its own - a group with its /Kids
    * already in place. Every node of that subtree is held to the same
    * rules as the child, under the fully qualified names the hierarchy
    * will give them once the child is in place.
    *
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE when the child's
    * dictionary is a direct object, the child or a node of its subtree
    * already belongs to this hierarchy, or a fully qualified name the
    * subtree would take already exists - in this hierarchy or twice within
    * the subtree itself.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_AddRootChild(FieldTreeHandle* handle, FieldHandle* child);

    /**
    * \brief
    * Insert a top-level field at the given zero-based position among the
    * root /Fields entries.
    *
    * The index is a position in the /Fields array itself - the same space
    * as \ref FieldTree_GetRootChild as long as every entry is a field, not
    * the flat view - and an index equal to the array size appends. The order of the container
    * arrays affects the enumeration order of the hierarchy only; the tab
    * order of a page comes from its /Annots array. Otherwise identical to
    * \ref FieldTree_AddRootChild.
    *
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the index is out of
    * range, \ref VANILLAPDF_ERROR_PARAMETER_VALUE as for
    * \ref FieldTree_AddRootChild.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_InsertRootChild(FieldTreeHandle* handle, size_type index, FieldHandle* child);

    /**
    * \brief
    * Append a child field to a parent field of this hierarchy.
    *
    * The child's /Parent entry is set to the parent, and the parent's /Kids
    * array is created when the parent has none yet. Adding a child to a
    * field that is currently a terminal turns it into a group; the child's
    * fully qualified name is then prefixed with the parent's. Any field of
    * this hierarchy is a valid parent, whether it came from
    * \ref FieldTree_GetRootChild, \ref Field_GetChild, \ref FieldTree_GetField
    * or \ref Field_GetParent - with the caveat that Field_GetParent reports
    * the /Parent entry as written, which a damaged file may point outside
    * the hierarchy (refused here) or at the wrong node of it (accepted, and
    * the child lands under that node). A top-level field has no parent and
    * a sibling of it is added with \ref FieldTree_AddRootChild.
    *
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE as for
    * \ref FieldTree_AddRootChild, and when the parent does not belong to
    * this hierarchy, is a terminal field carrying widget annotations, or is
    * merged with its own widget annotation.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_AddChild(FieldTreeHandle* handle, FieldHandle* parent, FieldHandle* child);

    /**
    * \brief
    * Insert a child field at the given zero-based position among the
    * parent's /Kids entries.
    *
    * The index counts the parent's /Kids entries and an index equal to
    * \ref Field_GetChildCount appends. Otherwise identical to
    * \ref FieldTree_AddChild.
    *
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the index is out of
    * range, \ref VANILLAPDF_ERROR_PARAMETER_VALUE as for
    * \ref FieldTree_AddChild.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_InsertChild(FieldTreeHandle* handle, FieldHandle* parent, size_type index, FieldHandle* child);

    /**
    * \brief
    * Remove a field - top-level or nested - from the hierarchy.
    *
    * The field's reference is removed from the container the hierarchy
    * walk reached it through - the root /Fields array or its group's
    * /Kids - and its /Parent entry is cleared. The walk is the authority,
    * not the field's /Parent entry: a field whose /Parent is missing or
    * points elsewhere is still removed from where it actually is. A group
    * is removed together with its whole subtree. A group emptied by the
    * removal stays in place; remove it explicitly if it is no longer wanted.
    *
    * The widget annotations of the removed fields are not touched - they
    * stay in the page /Annots arrays. Remove them through
    * \ref PageAnnotations_Remove, otherwise the page keeps drawing widgets
    * of fields that no longer exist. A container that lists the same node
    * twice - which the enumeration reports once - loses the first entry
    * only.
    *
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE when the field does not
    * belong to this hierarchy, or when its container no longer holds it
    * because the hierarchy was edited underneath the tree without
    * \ref FieldTree_Invalidate.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_RemoveChild(FieldTreeHandle* handle, FieldHandle* field);

    /**
    * \brief
    * Discard the cached flat view, so the next access rebuilds it from the
    * dictionaries.
    *
    * The tree caches the resolved terminal fields and keeps that cache
    * valid across its own mutators. It cannot observe edits made underneath
    * it through the dictionary API - via \ref Field_GetBaseObject or any
    * other path to the /Fields, /Kids and /Parent entries. Call this after
    * such edits and before the next enumeration or lookup.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_Invalidate(FieldTreeHandle* handle);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_ToUnknown(FieldTreeHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FieldTreeHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_FromUnknown(IUnknownHandle* handle, FieldTreeHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldTree_Release(FieldTreeHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FIELD_TREE_H */
