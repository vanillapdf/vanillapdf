#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/files/xref_chain.h"

#include "vanillapdf/syntax/c_file.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION File_Open(string_type filename, FileHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        FilePtr file = File::Open(filename);
        auto ptr = file.AddRefGet();
        *result = reinterpret_cast<FileHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_OpenStream(InputOutputStreamHandle* input_stream, string_type name, FileHandle** result) {

    auto input_stream_converted = reinterpret_cast<IInputOutputStream*>(input_stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(input_stream_converted);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        FilePtr file = File::OpenStream(input_stream_converted, name);
        auto ptr = file.AddRefGet();
        *result = reinterpret_cast<FileHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_Create(string_type filename, FileHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        FilePtr file = File::Create(filename);
        auto ptr = file.AddRefGet();
        *result = reinterpret_cast<FileHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_CreateStream(InputOutputStreamHandle* input_stream, string_type name, FileHandle** result) {

    auto input_stream_converted = reinterpret_cast<IInputOutputStream*>(input_stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(input_stream_converted);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        FilePtr file = File::CreateStream(input_stream_converted, name);
        auto ptr = file.AddRefGet();
        *result = reinterpret_cast<FileHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle* handle)
{
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);

    try
    {
        file->Initialize();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_GetVersion(FileHandle* handle, PDFVersion* result) {

    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto file_header = file->GetHeader();
        auto version = file_header->GetVersion();

        switch (version) {
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
        case Version::PDF20:
            *result = PDFVersion_20; break;
        default:
            return VANILLAPDF_ERROR_NOT_SUPPORTED;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_GetFilename(FileHandle* handle, BufferHandle** result) {

    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto filename_buffer = file->GetFilenameData();
        auto ptr = filename_buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle* handle, XrefChainHandle** result)
{
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto chain = file->GetXrefChain();
        auto ptr = chain.AddRefGet();
        *result = reinterpret_cast<XrefChainHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_GetIndirectObject(
    FileHandle* handle, biguint_type obj_number, ushort_type gen_number, ObjectHandle** result)
{
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto item = file->GetIndirectObject(obj_number, gen_number);
        auto ptr = item.AddRefGet();

        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_AllocateNewEntry(FileHandle* handle, XrefUsedEntryHandle** result) {
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto entry = file->AllocateNewEntry();
        auto ptr = entry.AddRefGet();

        *result = reinterpret_cast<XrefUsedEntryHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_Release(FileHandle* handle)
{
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);

    file->Release();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION File_IsEncrypted(FileHandle* handle, boolean_type* result)
{
    File* file = reinterpret_cast<File*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = file->IsEncrypted();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_SetEncryptionPassword(FileHandle* handle, string_type password)
{
    File* file = reinterpret_cast<File*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);

    try
    {
        std::string str;
        if (password != nullptr) {
            str = password;
        }

        bool result = file->SetEncryptionPassword(str);
        if (true == result) {
            return VANILLAPDF_ERROR_SUCCESS;
        }
        else {
            return VANILLAPDF_ERROR_INVALID_PASSWORD;
        }
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_SetEncryptionKey(FileHandle* handle, EncryptionKeyHandle* key)
{
    File* file = reinterpret_cast<File*>(handle);
    IEncryptionKey* encryption_key = reinterpret_cast<IEncryptionKey*>(key);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(encryption_key);

    try
    {
        bool result = file->SetEncryptionKey(*encryption_key);
        if (true == result) {
            return VANILLAPDF_ERROR_SUCCESS;
        }
        else {
            return VANILLAPDF_ERROR_INVALID_PASSWORD;
        }
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION File_ToUnknown(FileHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<File, IUnknown, FileHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION File_FromUnknown(IUnknownHandle* handle, FileHandle** result) {
    return SafeObjectConvert<IUnknown, File, IUnknownHandle, FileHandle>(handle, result);
}
