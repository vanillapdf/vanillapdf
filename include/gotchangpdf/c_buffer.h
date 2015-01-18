#ifndef _C_BUFFER_H
#define _C_BUFFER_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetSize(BufferHandle handle, out_size_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle handle, out_string_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_BUFFER_H */
