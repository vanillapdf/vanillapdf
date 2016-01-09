#include "precompiled.h"
#include "file.h"
#include "xref_chain.h"

#include "c_file.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(string_type filename, PFileHandle result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_SCOPE(filename);

		try
		{
			FileHolderPtr holder(pdf_new FileHolder(filename));
			auto ptr = holder.AddRefGet();
			*result = reinterpret_cast<FileHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);

	try
	{
		auto file = holder->Value();
		if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
		LOG_SCOPE(file->GetFilename());

		try
		{
			file->Initialize();
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, PXrefChainHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto file = holder->Value();
		if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
		LOG_SCOPE(file->GetFilename());

		try
		{
			auto chain = file->GetXrefChain();
			auto ptr = chain.AddRefGet();
			*result = reinterpret_cast<XrefChainHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, integer_type objNumber, ushort_type genNumber, PObjectHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto file = holder->Value();
		if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
		LOG_SCOPE(file->GetFilename());

		try
		{
			auto item = file->GetIndirectObject(objNumber, genNumber);
			auto ptr = item.AddRefGet();

			*result = reinterpret_cast<ObjectHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);

	try
	{
		auto file = holder->Value();
		if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
		LOG_SCOPE(file->GetFilename());

		holder->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}
