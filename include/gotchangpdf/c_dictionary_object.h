#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryIteratorHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryIteratorHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryIteratorHandle handle, DictionaryHandle parent, out_boolean_type result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryIteratorHandle handle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryIteratorHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryHandle handle, NameHandle str, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryHandle handle, NameHandle str, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Iterator(DictionaryHandle handle, PDictionaryIteratorHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryHandle handle, NameHandle str);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryHandle handle, NameHandle str, ObjectHandle value);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
