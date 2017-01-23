#include "precompiled.h"
#include "syntax/files/file.h"
#include "syntax/files/file_writer.h"
#include "syntax/files/xref_chain.h"

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
		FilePtr file = File::Open(filename);
		auto ptr = file.AddRefGet();
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
		FilePtr file = File::Create(filename);
		auto ptr = file.AddRefGet();
		*result = reinterpret_cast<FileHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);

	try
	{
		file->Initialize();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, XrefChainHandle* result)
{
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

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
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

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
	File* file = reinterpret_cast<File*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);

	file->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle handle, boolean_type* result)
{
	File* file = reinterpret_cast<File*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = file->IsEncrypted();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle handle, string_type password)
{
	File* file = reinterpret_cast<File*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(password);

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
	File* file = reinterpret_cast<File*>(handle);
	IEncryptionKey* encryption_key = reinterpret_cast<IEncryptionKey*>(key);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(encryption_key);


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
	File* source_file = reinterpret_cast<File*>(source);
	File* destination_file = reinterpret_cast<File*>(destination);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file);

	try
	{
		writer->Write(source_file, destination_file);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle handle, FileHandle source, FileHandle destination)
{
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	File* source_file = reinterpret_cast<File*>(source);
	File* destination_file = reinterpret_cast<File*>(destination);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file);

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