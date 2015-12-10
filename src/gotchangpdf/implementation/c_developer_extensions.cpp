#include "precompiled.h"
#include "developer_extensions.h"

#include "c_developer_extensions.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_GetBaseVersion(DeveloperExtensionsHandle handle, PPDFVersion result)
{
	DeveloperExtensions* obj = reinterpret_cast<DeveloperExtensions*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		switch (obj->BaseVersion()) {
		case Version::PDF10:
			*result = PDFVersion_10; break;
		case Version::PDF11:
			*result = PDFVersion_11; break;
		case Version::PDF12:
			*result = PDFVersion_12; break;
		case Version::PDF13:
			*result = PDFVersion_13; break;
		case Version::PDF14:
			*result = PDFVersion_14; break;
		case Version::PDF15:
			*result = PDFVersion_15; break;
		case Version::PDF16:
			*result = PDFVersion_16; break;
		case Version::PDF17:
			*result = PDFVersion_17; break;
		default:
			return GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_GetExtensionLevel(DeveloperExtensionsHandle handle, PIntegerHandle result)
{
	DeveloperExtensions* obj = reinterpret_cast<DeveloperExtensions*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		auto level = obj->ExtensionLevel();
		auto ptr = level.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle)
{
	DeveloperExtensions* obj = reinterpret_cast<DeveloperExtensions*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}