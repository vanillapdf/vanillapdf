#include "file.h"
#include "xref.h"
#include "indirect_object.h"
#include "catalog.h"
#include "log.h"

#include "c_file.h"
#include "c_helper.h"

using namespace gotchangpdf::files;

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Create(const char *filename, PFileHandle result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	LOG_SCOPE(filename);

	try
	{
		auto file = new File(filename);
		*result = reinterpret_cast<FileHandle>(file);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	if (nullptr != file) {
		LOG_SCOPE(file->GetFilename());
		delete file;
	}
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	LOG_SCOPE(file->GetFilename());

	try
	{
		file->Initialize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Xref(FileHandle handle, PXrefHandle result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(file->GetFilename());

	try
	{
		auto table = file->GetXref();
		auto ptr = table.AddRefGet();

		*result = reinterpret_cast<XrefHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, int objNumber, int genNumber, PIndirectHandle result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(file->GetFilename());

	try
	{
		auto item = file->GetIndirectObject(objNumber, genNumber);
		auto ptr = item.AddRefGet();

		//boost::intrusive_ptr_add_ref(ptr);
		*result = reinterpret_cast<IndirectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetDocumentCatalog(
	FileHandle handle, PCatalogHandle result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(file->GetFilename());

	try
	{
		auto item = file->GetDocumentCatalog();
		auto ptr = item.AddRefGet();

		//boost::intrusive_ptr_add_ref(ptr);
		*result = reinterpret_cast<CatalogHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
