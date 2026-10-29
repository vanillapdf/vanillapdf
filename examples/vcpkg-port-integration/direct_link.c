/*
* Consumes the library the way a project without CMake does, such as one using
* the vcpkg MSBuild integration: the include directory and the import library,
* nothing else. No vanillapdf::vanillapdf target, so no compile definitions.
*
* The headers alone have to select __declspec(dllimport). The exported
* constants below cannot be linked from a DLL without it.
*/

#include <vanillapdf/c_vanillapdf_api.h>

#include <stdio.h>

/*
* The test runs against a shared library only. A static library links
* without dllimport, so the test would pass without proving anything.
*/
#if !VANILLAPDF_SHARED_LIBRARY
    #error "The installed library is not a shared library, the direct link test needs one"
#endif

int main(void) {
    error_type result = VANILLAPDF_ERROR_GENERAL;
    integer_type major = 0;
    integer_type minor = 0;
    integer_type patch = 0;

    printf("vcpkg Port Direct Link Test\n");
    printf("===========================\n");

    /* Exported data, unresolved at link time without dllimport */
    if (VANILLAPDF_RV_TRUE == VANILLAPDF_RV_FALSE) {
        fprintf(stderr, "Exported boolean constants are not distinct\n");
        return 1;
    }

    if (VANILLAPDF_ERROR_SUCCESS == VANILLAPDF_ERROR_GENERAL) {
        fprintf(stderr, "Exported error constants are not distinct\n");
        return 1;
    }

    /* Exported functions */
    result = LibraryInfo_GetVersionMajor(&major);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Failed to get major version (error: %u)\n", (unsigned) result);
        return 1;
    }

    result = LibraryInfo_GetVersionMinor(&minor);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Failed to get minor version (error: %u)\n", (unsigned) result);
        return 1;
    }

    result = LibraryInfo_GetVersionPatch(&patch);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Failed to get patch version (error: %u)\n", (unsigned) result);
        return 1;
    }

    printf("Library version: %d.%d.%d\n", (int) major, (int) minor, (int) patch);
    printf("Direct link integration successful\n");

    return 0;
}
