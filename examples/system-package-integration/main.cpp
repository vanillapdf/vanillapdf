#include <iostream>
#include <vanillapdf/c_vanillapdf_api.h>

// Deliberately minimal: this example exists to validate how the package is laid
// out on disk, not what the library can do. Reading the version is enough to
// prove that the headers were found, that the import resolved against the
// installed library, and that the loader can find it at run time.

int main() {
    integer_type major = 0;
    integer_type minor = 0;
    integer_type patch = 0;

    if (LibraryInfo_GetVersionMajor(&major) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionMinor(&minor) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionPatch(&patch) != VANILLAPDF_ERROR_SUCCESS) {
        std::cerr << "Failed to query the vanillapdf version\n";
        return 1;
    }

    std::cout << "vanillapdf " << major << "." << minor << "." << patch << "\n";
    std::cout << "System package integration successful\n";

    return 0;
}
