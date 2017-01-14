#include "precompiled.h"
#include "file.h"
#include "file_writer.h"
#include "xref_chain.h"

#include "gotchangpdf/c_file.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(string_type filename, FileHandle* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		FileHolderPtr holder;
		holder->Open(filename);
		auto ptr = holder.AddRefGet();
		*result = reinterpret_cast<FileHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Create(string_type filename, FileHandle* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		FileHolderPtr holder;
		holder->Create(filename);
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, XrefChainHandle* result)
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
	FileHandle handle, biguint_type objNumber, ushort_type genNumber, ObjectHandle* result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, boolean_type* result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Create(FileWriterHandle* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		FileWriterPtr writer;
		auto ptr = writer.AddRefGet();
		*result = reinterpret_cast<FileWriterHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Write(FileWriterHandle handle, FileHandle source, FileHandle destination)
{
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	FileHolder* source_file_holder = reinterpret_cast<FileHolder*>(source);
	FileHolder* destination_file_holder = reinterpret_cast<FileHolder*>(destination);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file_holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file_holder);

	auto source_file = source_file_holder->Value();
	auto destination_file = destination_file_holder->Value();
	if (!source_file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
	if (!destination_file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		writer->Write(source_file, destination_file);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle handle, FileHandle source, FileHandle destination)
{
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	FileHolder* source_file_holder = reinterpret_cast<FileHolder*>(source);
	FileHolder* destination_file_holder = reinterpret_cast<FileHolder*>(destination);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file_holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file_holder);

	auto source_file = source_file_holder->Value();
	auto destination_file = destination_file_holder->Value();
	if (!source_file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;
	if (!destination_file) return GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	try
	{
		writer->WriteIncremental(source_file, destination_file);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_Release(FileWriterHandle handle)
{
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);

	writer->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}