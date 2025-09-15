#include <iostream>
#include <vanillapdf/api.h>

int main() {
    std::cout << "FetchContent Integration Test\n";
    std::cout << "============================\n";

    // Test basic VanillaPDF functionality
    try {
        // Test creating a PDF document
        auto doc = vanillapdf::CreateDocument();
        if (!doc) {
            std::cerr << "❌ Failed to create PDF document\n";
            return 1;
        }

        std::cout << "✅ Successfully created PDF document via FetchContent\n";
        std::cout << "✅ VanillaPDF is properly linked and functional\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Exception: " << e.what() << "\n";
        return 1;
    }
}