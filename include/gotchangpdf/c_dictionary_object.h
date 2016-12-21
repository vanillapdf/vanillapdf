#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle handle, PNameObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle parent, out_boolean_type result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle);

	/**
	* \memberof DictionaryObjectIterator
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, NameObjectHandle key, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryObjectHandle handle, NameObjectHandle key, out_boolean_type result);
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle, PDictionaryObjectIteratorHandle result);

	/**
	* \memberof DictionaryObject
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryObjectHandle handle, NameObjectHandle key);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryObjectHandle handle, NameObjectHandle key, ObjectHandle value);
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
