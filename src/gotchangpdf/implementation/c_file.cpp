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
		FileHolderPtr holder(pdf_new FileHolder(filename));
		auto ptr = holder.AddRefGet();
		*result = reinterpret_cast<FileHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		file->Initialize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, PXrefChainHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		auto chain = file->GetXrefChain();
		auto ptr = chain.AddRefGet();
		*result = reinterpret_cast<XrefChainHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, uinteger_type objNumber, ushort_type genNumber, PObjectHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		auto item = file->GetIndirectObject(objNumber, genNumber);
		auto ptr = item.AddRefGet();

		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	holder->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, out_boolean_type result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		*result = file->IsEncrypted();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle handle, string_type password)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(password);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		std::string str(password);
		bool result = file->SetEncryptionPassword(str);
		if (true == result)
			return GOTCHANG_PDF_ERROR_SUCCES;
		else
			return GOTCHANG_PDF_ERROR_INVALID_PASSWORD;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionKey(FileHandle handle, EncryptionKeyHandle key)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(handle);
	IEncryptionKey* encryption_key = reinterpret_cast<IEncryptionKey*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(encryption_key);

	auto file = holder->Value();
	if (!file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		bool result = file->SetEncryptionKey(*encryption_key);
		if (true == result)
			return GOTCHANG_PDF_ERROR_SUCCES;
		else
			return GOTCHANG_PDF_ERROR_INVALID_PASSWORD;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}