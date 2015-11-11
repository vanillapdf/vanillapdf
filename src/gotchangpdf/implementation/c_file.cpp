#include "precompiled.h"
#include "file.h"
#include "xref_chain.h"
#include "catalog.h"
#include "object_visitors.h"

#include "c_file.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::files;

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(const char *filename, PFileHandle result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	LOG_SCOPE(file->GetFilename());

	delete file;
	return GOTCHANG_PDF_ERROR_SUCCES;
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, PXrefChainHandle result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(file->GetFilename());

	try
	{
		auto chain = file->GetXrefChain();
		auto ptr = chain.AddRefGet();
		*result = reinterpret_cast<XrefChainHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, integer_type objNumber, ushort_type genNumber, PObjectHandle result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(file->GetFilename());

	try
	{
		auto item = file->GetIndirectObject(objNumber, genNumber);
		ObjectBaseAddRefVisitor visitor;
		auto ptr = item.apply_visitor(visitor);

		*result = reinterpret_cast<ObjectHandle>(ptr);
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

		*result = reinterpret_cast<CatalogHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
