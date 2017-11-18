#include "precompiled.h"

#include "syntax/files/file_writer.h"

#include "vanillapdf/syntax/c_file_writer.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Create(FileWriterHandle* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		FileWriterPtr writer;
		auto ptr = writer.AddRefGet();
		*result = reinterpret_cast<FileWriterHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Write(FileWriterHandle handle, FileHandle source, FileHandle destination) {
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	File* source_file = reinterpret_cast<File*>(source);
	File* destination_file = reinterpret_cast<File*>(destination);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file);

	try {
		writer->Write(source_file, destination_file);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_WriteIncremental(FileWriterHandle handle, FileHandle source, FileHandle destination) {
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	File* source_file = reinterpret_cast<File*>(source);
	File* destination_file = reinterpret_cast<File*>(destination);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(destination_file);

	try {
		writer->WriteIncremental(source_file, destination_file);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileWriter_Release(FileWriterHandle handle) {
	FileWriter* writer = reinterpret_cast<FileWriter*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(writer);

	writer->Release();
	return VANILLAPDF_ERROR_SUCCESS;
}
