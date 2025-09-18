#include <iostream>
#include <vanillapdf/c_vanillapdf_api.h>

int main() {
    std::cout << "FetchContent Integration Test\n";
    std::cout << "============================\n";

    // Test basic VanillaPDF functionality
    FileHandle* file_handle = nullptr;

    // Test creating a PDF file (this tests the API is working)
    error_type result = File_Create("test_output.pdf", &file_handle);

    if (result != ERROR_SUCCESS) {
        std::cerr << "❌ Failed to create PDF file (error: " << result << ")\n";
        return 1;
    }

    if (!file_handle) {
        std::cerr << "❌ File handle is null\n";
        return 1;
    }

    // Clean up
    File_Release(file_handle);

    std::cout << "✅ Successfully created PDF file via FetchContent\n";
    std::cout << "✅ VanillaPDF is properly linked and functional\n";
    std::cout << "✅ API functions are accessible and working\n";

    return 0;
}