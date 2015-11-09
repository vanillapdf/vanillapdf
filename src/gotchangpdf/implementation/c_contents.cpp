#include "precompiled.h"
#include "contents.h"

#include "c_contents.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::documents;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetOperationsSize(ContentsHandle handle, out_integer_type result)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = obj->GetOperationsSize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_GetOperationAt(ContentsHandle handle, integer_type at)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		auto aa = obj->GetOperationAt(at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Contents_Release(ContentsHandle handle)
{
	Contents* obj = reinterpret_cast<Contents*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
