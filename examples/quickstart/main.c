#include <stdio.h>
#include <vanillapdf/c_vanillapdf_api.h>

int main(void) {
    integer_type major = 0;
    integer_type minor = 0;
    integer_type patch = 0;

    if (LibraryInfo_GetVersionMajor(&major) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionMinor(&minor) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionPatch(&patch) != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Could not query the library version\n");
        return 1;
    }

    printf("Vanilla.PDF %d.%d.%d\n", (int) major, (int) minor, (int) patch);
    return 0;
}
