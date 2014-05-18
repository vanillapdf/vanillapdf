#ifndef _C_DICTIONARY_OBJECT_H
#define _C_DICTIONARY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API NameObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle);
	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle);
	GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle, DictionaryObjectHandle);

	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle);

	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle, const char *str, int len);
	GOTCHANG_PDF_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DICTIONARY_OBJECT_H */
