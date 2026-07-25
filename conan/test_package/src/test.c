#include <stdio.h>
#include <vanillapdf/c_vanillapdf_api.h>

int main(void) {
    integer_type major = 0;
    integer_type minor = 0;
    integer_type patch = 0;

    /* Querying the linked library version proves the headers, the linkage and
     * the C API all work, without writing anything to the filesystem. */
    if (LibraryInfo_GetVersionMajor(&major) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionMinor(&minor) != VANILLAPDF_ERROR_SUCCESS
        || LibraryInfo_GetVersionPatch(&patch) != VANILLAPDF_ERROR_SUCCESS) {
        fprintf(stderr, "Failed to query the vanillapdf version\n");
        return 1;
    }

    printf("vanillapdf %d.%d.%d\n", (int)major, (int)minor, (int)patch);
    return 0;
}
