#ifndef _VANILLAPDF_UNITTEST_TEMP_FILE_H
#define _VANILLAPDF_UNITTEST_TEMP_FILE_H

#include <filesystem>
#include <string>

/**
 * RAII wrapper for temporary test files.
 *
 * Generates a unique path in the system temp directory and removes the file
 * when the guard goes out of scope. Ensures cleanup even when assertions
 * fail mid-test.
 *
 * Usage:
 *   TempFile tmp("vanillapdf_test_create_memory.pdf");
 *   Document_CreateWithStrategy(tmp.c_str(), IOStrategy_Memory, doc.out());
 *   // file is automatically removed at scope exit
 */
class TempFile {
    std::string path_;

public:
    explicit TempFile(const std::string& filename) {
        static const auto temp_dir = std::filesystem::temp_directory_path();
        auto full_path = temp_dir / filename;
        path_ = full_path.string();
    }

    ~TempFile() {
        std::error_code ec;
        std::filesystem::remove(path_, ec);
    }

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    const char* c_str() const { return path_.c_str(); }
    const std::string& str() const { return path_; }
};

#endif /* _VANILLAPDF_UNITTEST_TEMP_FILE_H */
