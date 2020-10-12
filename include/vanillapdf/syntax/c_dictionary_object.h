#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_dictionary_object.h
	* \brief This file contains class definitions for \ref DictionaryObjectHandle
	*/

	/**
	* \class DictionaryObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Represents key-value map.
	*/

	/**
	* \class DictionaryObjectIteratorHandle
	* \extends IUnknownHandle
	* \ingroup group_objects
	* \brief Used for accessing Dictionary elements
	* through iterator interface.
	*/

	/**
	* \memberof DictionaryObjectIteratorHandle
	* @{
	*/

	/**
	* \brief Get key at iterator position
	*
	* Ensure the iterator is valid.
	* \see \ref DictionaryObjectIterator_IsValid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle* handle, NameObjectHandle** result);

	/**
	* \brief Get value at iterator position
	*
	* Ensure the iterator is valid.
	* \see \ref DictionaryObjectIterator_IsValid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle* handle, ObjectHandle** result);

	/**
	* \brief Determine if current position is valid
	*
	* Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
	*
	* Any other operation except \b THIS will fail on invalid iterator.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle* handle, boolean_type* result);

	/**
	* \brief Advances iterator to the next position
	*
	* Ensure the iterator is valid.
	* \see \ref DictionaryObjectIterator_IsValid
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle* handle);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_ToUnknown(DictionaryObjectIteratorHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref DictionaryObjectIterator
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_FromUnknown(IUnknownHandle* handle, DictionaryObjectIteratorHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle* handle);

	/** @} */

	/**
	* \memberof DictionaryObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new dictionary instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Create(DictionaryObjectHandle** result);

	/**
	* \brief Return size of collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_GetSize(DictionaryObjectHandle* handle, size_type* result);

	/**
	* \brief Find mapped value for key \p key
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle* handle, const NameObjectHandle* key, ObjectHandle** result);

	/**
	* \brief Determine if collection contains \p key
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryObjectHandle* handle, const NameObjectHandle* key, boolean_type* result);

	/**
	* \brief Get collection iterator for enumerating all entries
	*
	* Modifying collection may invalidate this handle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_GetIterator(DictionaryObjectHandle* handle, DictionaryObjectIteratorHandle** result);

	/**
	* \brief Remove key-value pair from collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryObjectHandle* handle, const NameObjectHandle* key, boolean_type* result);

	/**
	* \brief Remove all items from the collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Clear(DictionaryObjectHandle* handle);

	/**
	* \brief Insert new key-value pair into collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryObjectHandle* handle, NameObjectHandle* key, ObjectHandle* value);

	/**
	* \copydoc DictionaryObject_Insert
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_InsertConst(DictionaryObjectHandle* handle, const NameObjectHandle* key, ObjectHandle* value);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_ToObject(DictionaryObjectHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref DictionaryObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_FromObject(ObjectHandle* handle, DictionaryObjectHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
