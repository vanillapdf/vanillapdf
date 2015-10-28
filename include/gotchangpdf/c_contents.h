#ifndef _C_CONTENTS_H
#define _C_CONTENTS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	//GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Operations(ContentsHandle handle, ContentsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENTS_H */
