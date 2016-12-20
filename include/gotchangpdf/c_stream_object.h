#ifndef _C_STREAM_OBJECT_H
#define _C_STREAM_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetHeader(StreamHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetHeader(StreamHandle handle, DictionaryHandle value);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBodyRaw(StreamHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBody(StreamHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetBody(StreamHandle handle, BufferHandle value);

	/**
	* \memberof StreamObject
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STREAM_OBJECT_H */
