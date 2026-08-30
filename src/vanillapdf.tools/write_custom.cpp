#include "tools.h"

namespace {

struct WriteCustomOptions {
    std::string source_file;
    std::string destination_file;
};

// Custom callbacks
struct ObserverData {
    int data = 0;
};

error_type on_initializing(void* user_data, InputOutputStreamHandle* input_output_stream) {
    ObserverData* observer_data = (ObserverData*) user_data;
    offset_type current_offset = 0;
    long long current_offset_converted = 0;

    OutputStreamGuard output_stream;

    UNUSED(observer_data);

    // Do something with the output stream
    RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_ToOutputStream(input_output_stream, output_stream.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_GetOutputPosition(output_stream, &current_offset));

    // Convert to long long for printf
    current_offset_converted = current_offset;

    printf("File writer is initializing\n");
    printf("Current output position: %lld\n", current_offset_converted);

    return VANILLAPDF_ERROR_SUCCESS;
}

error_type on_finalizing(void* user_data, InputOutputStreamHandle* input_output_stream) {
    ObserverData* observer_data = (ObserverData*) user_data;
    offset_type current_offset = 0;
    long long current_offset_converted = 0;

    OutputStreamGuard output_stream;

    UNUSED(observer_data);

    // Do something with the output stream
    RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_ToOutputStream(input_output_stream, output_stream.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_GetOutputPosition(output_stream, &current_offset));

    // Convert to long long for printf
    current_offset_converted = current_offset;

    printf("File writer is finalizing\n");
    printf("Current output position: %lld\n", current_offset_converted);

    return VANILLAPDF_ERROR_SUCCESS;
}

int process_write_custom(const WriteCustomOptions& options) {
    FileWriterGuard file_writer;
    FileGuard source_file;
    FileGuard destination_file;

    ObserverData observer_data;
    FileWriterObserverGuard file_writer_observer;

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), source_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Create(options.destination_file.c_str(), destination_file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(source_file));

    // Unused callbacks can be set to nullptr
    RETURN_ERROR_IF_NOT_SUCCESS(FileWriterObserver_CreateCustom(on_initializing, on_finalizing, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &observer_data, file_writer_observer.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Create(file_writer.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Subscribe(file_writer, file_writer_observer));
    RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Write(file_writer, source_file, destination_file));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_write_custom(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<WriteCustomOptions>();
    auto* command = app.add_subcommand("write_custom", "Write a PDF with custom file writer callbacks");

    command->add_option("-s,--source", options->source_file, "Source file")->required();
    command->add_option("-d,--destination", options->destination_file, "Destination file")->required();

    command->callback([options, &exit_code]() {
        exit_code = process_write_custom(*options);
    });
}
