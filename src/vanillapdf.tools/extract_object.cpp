#include "tools.h"

namespace {

struct ExtractObjectOptions {
    std::string source_file;
    biguint_type object_number = 0;
    ushort_type generation_number = 0;
};

error_type eo_process_stream(StreamObjectHandle* stream, biguint_type object_number, ushort_type generation_number) {
    BufferGuard decoded_body;
    OutputStreamGuard output_stream;

    std::string output_filename = std::to_string(object_number) + "." + std::to_string(generation_number);

    RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBody(stream, decoded_body.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename.c_str(), output_stream.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, decoded_body));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_object(ObjectHandle* obj, biguint_type object_number, ushort_type generation_number) {

    ObjectType type = ObjectType_Undefined;

    RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(obj, &type));

    if (type == ObjectType_Stream) {
        StreamObjectGuard stream;

        RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_FromObject(obj, stream.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(eo_process_stream(stream, object_number, generation_number));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_xref(XrefHandle* xref, biguint_type object_number, ushort_type generation_number) {
    XrefIteratorGuard xref_iterator;
    boolean_type valid = VANILLAPDF_RV_FALSE;

    RETURN_ERROR_IF_NOT_SUCCESS(Xref_GetIterator(xref, xref_iterator.out()));

    while (VANILLAPDF_ERROR_SUCCESS == XrefIterator_IsValid(xref_iterator, &valid)
        && VANILLAPDF_RV_TRUE == valid) {

        XrefEntryType type = XrefEntryType_Null;
        XrefEntryGuard entry;

        biguint_type current_object_number = 0;
        ushort_type current_generation_number = 0;

        RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, entry.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetType(entry, &type));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetObjectNumber(entry, &current_object_number));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetGenerationNumber(entry, &current_generation_number));

        if (current_object_number == object_number && current_generation_number == generation_number) {

            if (type == XrefEntryType_Used) {
                XrefUsedEntryGuard used_entry;
                ObjectGuard obj;

                RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_FromEntry(entry, used_entry.out()));
                RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_GetReference(used_entry, obj.out()));
                RETURN_ERROR_IF_NOT_SUCCESS(eo_process_object(obj, object_number, generation_number));
            }

            if (type == XrefEntryType_Compressed) {
                XrefCompressedEntryGuard compressed_entry;
                ObjectGuard obj;

                RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_FromEntry(entry, compressed_entry.out()));
                RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_GetReference(compressed_entry, obj.out()));
                RETURN_ERROR_IF_NOT_SUCCESS(eo_process_object(obj, object_number, generation_number));
            }
        }

        RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type eo_process_file(FileHandle* file, biguint_type object_number, ushort_type generation_number) {
    XrefChainGuard chain;
    XrefChainIteratorGuard chain_iterator;
    boolean_type valid = VANILLAPDF_RV_FALSE;

    RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, chain.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_GetIterator(chain, chain_iterator.out()));

    while (VANILLAPDF_ERROR_SUCCESS == XrefChainIterator_IsValid(chain_iterator, &valid)
        && VANILLAPDF_RV_TRUE == valid) {
        XrefGuard xref;

        RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, xref.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(eo_process_xref(xref, object_number, generation_number));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_extract_object(const ExtractObjectOptions& options) {
    FileGuard file;

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
    RETURN_ERROR_IF_NOT_SUCCESS(eo_process_file(file, options.object_number, options.generation_number));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_extract_object(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<ExtractObjectOptions>();
    auto* command = app.add_subcommand("extract_object", "Extract the decoded body of a stream object");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-o,--object", options->object_number, "Object number")->required();
    command->add_option("-g,--generation", options->generation_number, "Generation number")->capture_default_str();

    command->callback([options, &exit_code]() {
        exit_code = process_extract_object(*options);
    });
}
