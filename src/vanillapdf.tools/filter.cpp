#include "tools.h"

namespace {

struct FilterOptions {
    std::string filter_type;
    std::string operation;
    std::string source_file;
    std::string destination_file;
};

// Runs one encode/decode pass through the filter of the given handle type.
// The filter API is uniform - Create, Encode, Decode - so a single template
// covers every supported filter.
template <
    typename FilterHandle,
    error_type (CALLING_CONVENTION *ReleaseFn)(FilterHandle*),
    error_type (CALLING_CONVENTION *CreateFn)(FilterHandle**),
    error_type (CALLING_CONVENTION *EncodeFn)(FilterHandle*, BufferHandle*, BufferHandle**),
    error_type (CALLING_CONVENTION *DecodeFn)(FilterHandle*, BufferHandle*, BufferHandle**)
>
error_type apply_filter(bool is_encode, BufferHandle* input_data, OutputStreamHandle* output_stream) {
    HandleGuard<FilterHandle, ReleaseFn> filter_handle;
    BufferGuard output_data;

    RETURN_ERROR_IF_NOT_SUCCESS(CreateFn(filter_handle.out()));

    if (is_encode) {
        RETURN_ERROR_IF_NOT_SUCCESS(EncodeFn(filter_handle, input_data, output_data.out()));
    } else {
        RETURN_ERROR_IF_NOT_SUCCESS(DecodeFn(filter_handle, input_data, output_data.out()));
    }

    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, output_data));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_filter(const FilterOptions& options) {
    InputStreamGuard input_stream;
    OutputStreamGuard output_stream;
    BufferGuard input_data;

    bool is_encode = (options.operation == "encode");

    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(options.destination_file.c_str(), output_stream.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(InputStream_CreateFromFile(options.source_file.c_str(), input_stream.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(InputStream_ToBuffer(input_stream, input_data.out()));

    if (options.filter_type == "flate") {
        RETURN_ERROR_IF_NOT_SUCCESS((apply_filter<FlateDecodeFilterHandle, FlateDecodeFilter_Release,
            FlateDecodeFilter_Create, FlateDecodeFilter_Encode, FlateDecodeFilter_Decode>(is_encode, input_data, output_stream)));
    }

    if (options.filter_type == "dct") {
        RETURN_ERROR_IF_NOT_SUCCESS((apply_filter<DCTDecodeFilterHandle, DCTDecodeFilter_Release,
            DCTDecodeFilter_Create, DCTDecodeFilter_Encode, DCTDecodeFilter_Decode>(is_encode, input_data, output_stream)));
    }

    if (options.filter_type == "jpx") {
        RETURN_ERROR_IF_NOT_SUCCESS((apply_filter<JPXDecodeFilterHandle, JPXDecodeFilter_Release,
            JPXDecodeFilter_Create, JPXDecodeFilter_Encode, JPXDecodeFilter_Decode>(is_encode, input_data, output_stream)));
    }

    if (options.filter_type == "ascii85") {
        RETURN_ERROR_IF_NOT_SUCCESS((apply_filter<ASCII85DecodeFilterHandle, ASCII85DecodeFilter_Release,
            ASCII85DecodeFilter_Create, ASCII85DecodeFilter_Encode, ASCII85DecodeFilter_Decode>(is_encode, input_data, output_stream)));
    }

    if (options.filter_type == "ascii_hex") {
        RETURN_ERROR_IF_NOT_SUCCESS((apply_filter<ASCIIHexDecodeFilterHandle, ASCIIHexDecodeFilter_Release,
            ASCIIHexDecodeFilter_Create, ASCIIHexDecodeFilter_Encode, ASCIIHexDecodeFilter_Decode>(is_encode, input_data, output_stream)));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_filter(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<FilterOptions>();
    auto* command = app.add_subcommand("filter", "Encode or decode a file with a compression filter");

    command->add_option("-f,--filter", options->filter_type, "Filter type")
        ->required()
        ->check(CLI::IsMember({"flate", "dct", "jpx", "ascii85", "ascii_hex"}));
    command->add_option("-o,--operation", options->operation, "Operation")
        ->required()
        ->check(CLI::IsMember({"encode", "decode"}));
    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();

    command->callback([options, &exit_code]() {
        exit_code = process_filter(*options);
    });
}
