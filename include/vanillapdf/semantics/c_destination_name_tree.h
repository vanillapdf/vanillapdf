#ifndef _C_DESTINATION_NAME_TREE_H
#define _C_DESTINATION_NAME_TREE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_destination_name_tree.h
    * \brief This file contains class definitions for \ref DestinationNameTreeHandle
    */

    /**
    * \class DestinationNameTreeHandle
    * \extends IUnknownHandle
    * \ingroup group_destinations
    * \brief
    * A name tree that maps string names to destination objects.
    * This is the PDF 1.2+ method for storing named destinations,
    * replacing the old-style /Dests dictionary in the catalog.
    *
    * For more details please visit section 12.3.2.3 - Named Destinations.
    * \see \ref NameDictionaryHandle
    */

    /**
    * \class DestinationNameTreeIteratorHandle
    * \extends IUnknownHandle
    * \ingroup group_destinations
    * \brief
    * Iterator for traversing all entries in a \ref DestinationNameTreeHandle.
    */

    /**
    * \memberof DestinationNameTreeHandle
    * @{
    */

    /**
    * \brief Create a new empty destination name tree.
    *
    * The created tree can be populated using DestinationNameTree_Insert
    * and then assigned to a NameDictionary using NameDictionary_SetDestinations.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Create(DestinationNameTreeHandle** result);

    /**
    * \brief Determine if the name tree contains a destination with the given name.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Contains(DestinationNameTreeHandle* handle, StringObjectHandle* name, boolean_type* result);

    /**
    * \brief Find a destination by its string name.
    *
    * This function throws internal exception on failure.
    * Prefer using \ref DestinationNameTree_Contains or
    * \ref DestinationNameTree_TryFind for safer lookups.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Find(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle** result);

    /**
    * \brief Try to find a destination by its string name.
    *
    * \param handle The destination name tree handle
    * \param name The string name to search for
    * \param result Output: The destination handle if found, unchanged otherwise
    * \param found Output: VANILLAPDF_RV_TRUE if found, VANILLAPDF_RV_FALSE otherwise
    * \return VANILLAPDF_ERROR_SUCCESS on success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_TryFind(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle** result, boolean_type* found);

    /**
    * \brief Insert a named destination into the tree.
    *
    * \param handle The destination name tree handle
    * \param name The string name for the destination
    * \param destination The destination to insert
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Insert(DestinationNameTreeHandle* handle, StringObjectHandle* name, DestinationHandle* destination);

    /**
    * \brief Remove a named destination from the tree.
    *
    * \param handle The destination name tree handle
    * \param name The string name to remove
    * \param removed Output: VANILLAPDF_RV_TRUE if removed, VANILLAPDF_RV_FALSE if not found
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Remove(DestinationNameTreeHandle* handle, StringObjectHandle* name, boolean_type* removed);

    /**
    * \brief Get an iterator to traverse all named destinations.
    *
    * Modifying the collection may invalidate this handle.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_GetIterator(DestinationNameTreeHandle* handle, DestinationNameTreeIteratorHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_ToUnknown(DestinationNameTreeHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref DestinationNameTreeHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_FromUnknown(IUnknownHandle* handle, DestinationNameTreeHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTree_Release(DestinationNameTreeHandle* handle);

    /** @} */

    /**
    * \memberof DestinationNameTreeIteratorHandle
    * @{
    */

    /**
    * \brief Get the key (name) at the current iterator position.
    *
    * Ensure the iterator is valid.
    * \see \ref DestinationNameTreeIterator_IsValid
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_GetKey(DestinationNameTreeIteratorHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the key (name) at the current iterator position as a \ref PdfTextStringHandle.
    *
    * Ensure the iterator is valid.
    * \see \ref DestinationNameTreeIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_GetKeyText(DestinationNameTreeIteratorHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Get the value (destination) at the current iterator position.
    *
    * Ensure the iterator is valid.
    * \see \ref DestinationNameTreeIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_GetValue(DestinationNameTreeIteratorHandle* handle, DestinationHandle** result);

    /**
    * \brief Determine if current position is valid.
    *
    * Invalid position may mean that iterator moved past the end
    * or the collection was modified.
    *
    * Any other operation except \b THIS will fail on invalid iterator.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_IsValid(DestinationNameTreeIteratorHandle* handle, boolean_type* result);

    /**
    * \brief Advances iterator to the next position.
    *
    * Ensure the iterator is valid.
    * \see \ref DestinationNameTreeIterator_IsValid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_Next(DestinationNameTreeIteratorHandle* handle);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_ToUnknown(DestinationNameTreeIteratorHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref DestinationNameTreeIteratorHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_FromUnknown(IUnknownHandle* handle, DestinationNameTreeIteratorHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DestinationNameTreeIterator_Release(DestinationNameTreeIteratorHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DESTINATION_NAME_TREE_H */
