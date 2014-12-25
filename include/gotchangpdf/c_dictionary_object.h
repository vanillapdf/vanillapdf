#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API NameHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryIteratorHandle);
	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryIteratorHandle);
	GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryIteratorHandle, DictionaryHandle);

	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryIteratorHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryIteratorHandle);

	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryHandle, const char *str, int len);
	GOTCHANG_PDF_API DictionaryIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
