#include "tools.h"

void print_write_custom_help() {
	printf("Usage: write_custom -s [source file] -d [destination file]");
}

// Custom callbacks
typedef struct {
	int data;
} ObserverData;

error_type on_initializing(void* user_data, InputOutputStreamHandle* input_output_stream) {
	ObserverData* observer_data = (ObserverData*) user_data;
	offset_type current_offset = 0;
	long long current_offset_converted = 0;

	OutputStreamHandle* output_stream = NULL;

	UNUSED(observer_data);

	// Do something with the output stream
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_ToOutputStream(input_output_stream, &output_stream));
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

	OutputStreamHandle* output_stream = NULL;

	UNUSED(observer_data);

	// Do something with the output stream
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_ToOutputStream(input_output_stream, &output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_GetOutputPosition(output_stream, &current_offset));

	// Convert to long long for printf
	current_offset_converted = current_offset;

	printf("File writer is finalizing\n");
	printf("Current output position: %lld\n", current_offset_converted);

	return VANILLAPDF_ERROR_SUCCESS;
}

int process_write_custom(int argc, char *argv[]) {
	string_type source_file_path = NULL;
	string_type destination_file_path = NULL;

	FileWriterHandle* file_writer = NULL;
	FileHandle* source_file = NULL;
	FileHandle* destination_file = NULL;

	ObserverData observer_data = { 0 };
	FileWriterObserverHandle* file_writer_observer = NULL;

	integer_type i = 0;

	for (i = 0; i < argc; ++i) {

		// source document path
		if (strcmp(argv[i], "-s") == 0 && (i + 1 < argc)) {
			source_file_path = argv[i + 1];
			i++;

			// destination file path
		} else if (strcmp(argv[i], "-d") == 0 && (i + 1 < argc)) {
			destination_file_path = argv[i + 1];
			i++;
		} else {
			print_write_custom_help();
			return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
		}
	}

	if (source_file_path == NULL || destination_file_path == NULL) {
		print_write_custom_help();
		return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(source_file_path, &source_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Create(destination_file_path, &destination_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(source_file));

	// Unused callbacks can be set to NULL
	RETURN_ERROR_IF_NOT_SUCCESS(FileWriterObserver_CreateCustom(on_initializing, on_finalizing, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &observer_data, &file_writer_observer));

	RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Create(&file_writer));
	RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Subscribe(file_writer, file_writer_observer));
	RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Write(file_writer, source_file, destination_file));
	RETURN_ERROR_IF_NOT_SUCCESS(FileWriter_Release(file_writer));

	RETURN_ERROR_IF_NOT_SUCCESS(FileWriterObserver_Release(file_writer_observer));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(destination_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(source_file));

	return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}
