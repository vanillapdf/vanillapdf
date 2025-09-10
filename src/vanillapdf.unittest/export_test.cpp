#include "unittest.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <set>

namespace export_verification {

class ExportTest : public ::testing::Test {
protected:
    // Check if a symbol name contains special characters that indicate mangling
    bool HasMangledCharacters(const std::string& symbolName) {
        // Check for common C++ name mangling characters across platforms
        const std::set<char> manglingChars = {'?', '@', '$', '#', '%', '^', '&', '*', '(', ')', '[', ']', '{', '}', '<', '>'};
        
        for (char c : symbolName) {
            if (manglingChars.find(c) != manglingChars.end()) {
                return true;
            }
        }
        return false;
    }

    // Get the path to the built library
    std::string GetLibraryPath() {
#ifdef _WIN32
        return "vanillapdf.dll";
#elif defined(__APPLE__)
        return "libvanillapdf.dylib";
#else
        return "libvanillapdf.so";
#endif
    }

    // Run the Python verification script
    std::pair<bool, std::string> RunExportVerification() {
        std::string libraryPath = GetLibraryPath();
        std::string pythonScript = "../../../scripts/verify_exports.py";
        
        // Build command
        std::string command = "python3 " + pythonScript + " " + libraryPath + " 2>&1";
        
        // Execute command and capture output
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return {false, "Failed to run export verification script"};
        }
        
        std::string output;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            output += buffer;
        }
        
        int exitCode = pclose(pipe);
        bool success = (exitCode == 0);
        
        return {success, output};
    }
};

TEST_F(ExportTest, VerifyAllExportsUsingExternalTool) {
    auto [success, output] = RunExportVerification();
    
    // Print the output for debugging
    std::cout << "Export verification output:" << std::endl;
    std::cout << output << std::endl;
    
    EXPECT_TRUE(success) << "Export verification failed. Output: " << output;
}

} // namespace export_verification