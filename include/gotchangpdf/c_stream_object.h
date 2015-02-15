#ifndef _C_STREAM_OBJECT_H
#define _C_STREAM_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Header(StreamHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_BodyRaw(StreamHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_BodyDecoded(StreamHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STREAM_OBJECT_H */
