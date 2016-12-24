#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_dictionary_object.h
	* This file contains class definitions for DictionaryObjectHandle
	*/

	/**
	* \brief Get key at iterator position
	* \memberof DictionaryObjectIteratorHandle
	*
	* Ensure the iterator is valid.
	* \see DictionaryObjectIterator_IsValid()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle handle, PNameObjectHandle result);

	/**
	* \brief Get value at iterator position
	* \memberof DictionaryObjectIteratorHandle
	*
	* Ensure the iterator is valid.
	* \see DictionaryObjectIterator_IsValid()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle, PObjectHandle result);

	/**
	* \brief Determine if current position is valid
	* \memberof DictionaryObjectIteratorHandle
	*
	* Invalid position may mean that iterator moved past the end of the collection, as well as the collection was modified.
	*
	* Any other operation except \b THIS will fail on invalid iterator.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle parent, out_boolean_type result);

	/**
	* \brief Advances iterator to the next position
	* \memberof DictionaryObjectIteratorHandle
	*
	* Ensure the iterator is valid.
	* \see DictionaryObjectIterator_IsValid()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle);

	/**
	* \memberof DictionaryObjectIteratorHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle);

	/**
	* \brief Find mapped value for key \p key
	* \memberof DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, NameObjectHandle key, PObjectHandle result);

	/**
	* \brief Determine if collection contains \p key
	* \memberof DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryObjectHandle handle, NameObjectHandle key, out_boolean_type result);

	/**
	* \brief Get collection iterator for enumerating all entries
	*
	* Modifying collection may invalidate this handle.
	* \memberof DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle, PDictionaryObjectIteratorHandle result);

	/**
	* \brief Remove key-value pair from collection
	* \memberof DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryObjectHandle handle, NameObjectHandle key);

	/**
	* \brief Insert new key-value pair into collection
	* \memberof DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryObjectHandle handle, NameObjectHandle key, ObjectHandle value);

	/**
	* \memberof DictionaryObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
