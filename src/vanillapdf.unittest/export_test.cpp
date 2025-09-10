#include "unittest.h"

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#pragma comment(lib, "dbghelp.lib")
#endif

namespace export_verification {

#ifdef _WIN32

class ExportTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize symbol handler
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
    }

    void TearDown() override {
        // Cleanup symbol handler
        SymCleanup(GetCurrentProcess());
    }

    // Structure to hold export information
    struct ExportInfo {
        std::string name;
        DWORD address;
        WORD ordinal;
    };

    // Callback function for enumerating exports
    static BOOL CALLBACK EnumExportsCallback(PVOID pContext, ULONG Index, PSTR pszSymbolName, PVOID pSymbolAddress) {
        if (pContext && pszSymbolName) {
            auto* exports = static_cast<std::vector<ExportInfo>*>(pContext);
            ExportInfo info;
            info.name = pszSymbolName;
            info.address = reinterpret_cast<DWORD_PTR>(pSymbolAddress);
            info.ordinal = static_cast<WORD>(Index);
            exports->push_back(info);
        }
        return TRUE;
    }

    // Get exports from current module (DLL)
    std::vector<ExportInfo> GetModuleExports() {
        std::vector<ExportInfo> exports;
        
        // Get handle to current module
        HMODULE hModule = GetModuleHandle(L"vanillapdf.dll");
        if (!hModule) {
            // If we're in a static build or different name, try to get the current process
            hModule = GetModuleHandle(NULL);
        }
        
        if (!hModule) {
            return exports;
        }

        // Get DOS header
        PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
        if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
            return exports;
        }

        // Get NT headers
        PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
            reinterpret_cast<BYTE*>(hModule) + pDosHeader->e_lfanew);
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
            return exports;
        }

        // Get export directory
        DWORD exportRVA = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        if (exportRVA == 0) {
            return exports;
        }

        PIMAGE_EXPORT_DIRECTORY pExportDir = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
            reinterpret_cast<BYTE*>(hModule) + exportRVA);

        // Get arrays
        DWORD* pFunctions = reinterpret_cast<DWORD*>(reinterpret_cast<BYTE*>(hModule) + pExportDir->AddressOfFunctions);
        DWORD* pNames = reinterpret_cast<DWORD*>(reinterpret_cast<BYTE*>(hModule) + pExportDir->AddressOfNames);
        WORD* pOrdinals = reinterpret_cast<WORD*>(reinterpret_cast<BYTE*>(hModule) + pExportDir->AddressOfNameOrdinals);

        // Enumerate exports
        for (DWORD i = 0; i < pExportDir->NumberOfNames; ++i) {
            const char* pName = reinterpret_cast<const char*>(reinterpret_cast<BYTE*>(hModule) + pNames[i]);
            DWORD functionRVA = pFunctions[pOrdinals[i]];
            
            ExportInfo info;
            info.name = pName;
            info.address = functionRVA;
            info.ordinal = static_cast<WORD>(pOrdinals[i] + pExportDir->Base);
            exports.push_back(info);
        }

        return exports;
    }

    // Check if a symbol name contains special characters that indicate mangling
    bool HasMangledCharacters(const std::string& symbolName) {
        // Check for common C++ name mangling characters
        const std::set<char> manglingChars = {'?', '@', '$', '#', '%', '^', '&', '*', '(', ')', '[', ']', '{', '}', '<', '>'};
        
        for (char c : symbolName) {
            if (manglingChars.find(c) != manglingChars.end()) {
                return true;
            }
        }
        return false;
    }
};

TEST_F(ExportTest, VerifyNoMangledExports) {
    auto exports = GetModuleExports();
    
    // We should have some exports
    ASSERT_GT(exports.size(), 0) << "No exports found in the module";
    
    std::vector<std::string> mangledSymbols;
    std::vector<std::string> targetSymbols;
    
    // Check each export
    for (const auto& exportInfo : exports) {
        if (HasMangledCharacters(exportInfo.name)) {
            mangledSymbols.push_back(exportInfo.name);
        }
        
        // Specifically check for our target symbols
        if (exportInfo.name.find("NameConstant_LZWDecode") != std::string::npos ||
            exportInfo.name.find("NameConstant_JPXDecode") != std::string::npos) {
            targetSymbols.push_back(exportInfo.name);
        }
    }
    
    // Report findings
    if (!mangledSymbols.empty()) {
        std::cout << "Found " << mangledSymbols.size() << " mangled symbols:" << std::endl;
        for (const auto& symbol : mangledSymbols) {
            std::cout << "  " << symbol << std::endl;
        }
    }
    
    if (!targetSymbols.empty()) {
        std::cout << "Found target symbols:" << std::endl;
        for (const auto& symbol : targetSymbols) {
            std::cout << "  " << symbol << std::endl;
        }
    }
    
    // The test should pass - no mangled symbols should be found
    EXPECT_TRUE(mangledSymbols.empty()) << "Found " << mangledSymbols.size() << " mangled export symbols. All exported symbols should use C linkage and not contain special characters like @, ?, etc.";
    
    // Specifically verify that our target constants are exported without mangling
    bool foundLZWDecode = false;
    bool foundJPXDecode = false;
    
    for (const auto& exportInfo : exports) {
        if (exportInfo.name == "NameConstant_LZWDecode") {
            foundLZWDecode = true;
        }
        if (exportInfo.name == "NameConstant_JPXDecode") {
            foundJPXDecode = true;
        }
    }
    
    EXPECT_TRUE(foundLZWDecode) << "NameConstant_LZWDecode should be exported without name mangling";
    EXPECT_TRUE(foundJPXDecode) << "NameConstant_JPXDecode should be exported without name mangling";
}

TEST_F(ExportTest, VerifySpecificConstants) {
    auto exports = GetModuleExports();
    
    // List of constants that should be properly exported
    std::vector<std::string> expectedConstants = {
        "NameConstant_LZWDecode",
        "NameConstant_JPXDecode",
        "NameConstant_DCTDecode",
        "NameConstant_FlateDecode",
        "NameConstant_ASCII85Decode",
        "NameConstant_ASCIIHexDecode"
    };
    
    std::vector<std::string> missingConstants;
    std::vector<std::string> foundConstants;
    
    for (const auto& expectedConstant : expectedConstants) {
        bool found = false;
        for (const auto& exportInfo : exports) {
            if (exportInfo.name == expectedConstant) {
                found = true;
                foundConstants.push_back(expectedConstant);
                break;
            }
        }
        if (!found) {
            missingConstants.push_back(expectedConstant);
        }
    }
    
    if (!foundConstants.empty()) {
        std::cout << "Found properly exported constants:" << std::endl;
        for (const auto& constant : foundConstants) {
            std::cout << "  " << constant << std::endl;
        }
    }
    
    if (!missingConstants.empty()) {
        std::cout << "Missing constants:" << std::endl;
        for (const auto& constant : missingConstants) {
            std::cout << "  " << constant << std::endl;
        }
    }
    
    EXPECT_TRUE(missingConstants.empty()) << "All expected name constants should be properly exported";
    EXPECT_TRUE(std::find(foundConstants.begin(), foundConstants.end(), "NameConstant_LZWDecode") != foundConstants.end()) 
        << "NameConstant_LZWDecode must be exported";
    EXPECT_TRUE(std::find(foundConstants.begin(), foundConstants.end(), "NameConstant_JPXDecode") != foundConstants.end()) 
        << "NameConstant_JPXDecode must be exported";
}

#else

// For non-Windows platforms, create a placeholder test
TEST(ExportTest, PlatformNotSupported) {
    GTEST_SKIP() << "Export verification is only supported on Windows platforms";
}

#endif

} // namespace export_verification