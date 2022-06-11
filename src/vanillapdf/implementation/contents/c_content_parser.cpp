#include "precompiled.h"

#include "syntax/files/file.h"
#include "contents/content_stream_parser.h"

#include "vanillapdf/contents/c_content_parser.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::contents;

VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_Create(FileHandle* source_file, InputStreamHandle* input_stream, ContentParserHandle** result) {
	File* source_file_obj = reinterpret_cast<File*>(source_file);
	IInputStream* input_stream_obj = reinterpret_cast<IInputStream*>(input_stream);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(source_file_obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(input_stream_obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto parser = make_deferred<ContentStreamParser>(source_file_obj->GetWeakReference(), input_stream_obj);
		auto ptr = parser.AddRefGet();
		*result = reinterpret_cast<ContentParserHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_ReadInstructionCollection(ContentParserHandle* handle, ContentInstructionCollectionHandle** result) {
	ContentStreamParser* obj = reinterpret_cast<ContentStreamParser*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto instructions = obj->ReadInstructions();
		auto ptr = instructions.AddRefGet();
		*result = reinterpret_cast<ContentInstructionCollectionHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_ToUnknown(ContentParserHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<ContentStreamParser, IUnknown, ContentParserHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_FromUnknown(IUnknownHandle* handle, ContentParserHandle** result) {
	return SafeObjectConvert<IUnknown, ContentStreamParser, IUnknownHandle, ContentParserHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ContentParser_Release(ContentParserHandle* handle) {
	return ObjectRelease<ContentStreamParser, ContentParserHandle>(handle);
}
