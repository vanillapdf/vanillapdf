#include "tools.h"

/**
 * Extract the directory component from a file path into the provided buffer.
 * Returns the length of the directory string, or 0 if no separator was found.
 */
static size_t get_directory_from_path(const char* path, char* buffer, size_t buffer_size) {
    const char* fwd = strrchr(path, '/');
    const char* bck = strrchr(path, '\\');
    const char* sep = (bck > fwd) ? bck : fwd;

    if (sep == NULL) {
        return 0;
    }

    size_t dir_len = (size_t)(sep - path);
    if (dir_len >= buffer_size) {
        return 0;
    }

    memcpy(buffer, path, dir_len);
    buffer[dir_len] = '\0';
    return dir_len;
}

/**
 * Set OpenSSL provider modules path to the executable's directory.
 *
 * On shared builds with OpenSSL 3.x, the compiled-in MODULESDIR may not
 * match the actual location of provider modules (e.g. legacy.dll/.so).
 * Derives the directory from the executable path and configures the
 * OpenSSL provider search path so modules are found alongside the binary.
 */
static void configure_openssl_modules_path(const char* executable_path) {
    char dir[4096];
    size_t len = get_directory_from_path(executable_path, dir, sizeof(dir));
    const char* modules_path = (len > 0) ? dir : ".";

    error_type result = MiscUtils_SetOpenSSLModulesPath(modules_path);
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        printf("Warning: Failed to set OpenSSL modules path to '%s' (error %u)\n",
            modules_path, result);
    }
}

const int VANILLAPDF_TOOLS_ERROR_SUCCESS = 0;
const int VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS = 1;
const int VANILLAPDF_TOOLS_ERROR_FAILURE = 255;

void print_version() {
    printf("vanillapdf.tools %d.%d.%d\n",
        VANILLAPDF_TOOLS_VERSION_MAJOR,
        VANILLAPDF_TOOLS_VERSION_MINOR,
        VANILLAPDF_TOOLS_VERSION_PATCH);
}

void print_help() {
    print_version();
    printf("\nUsage: vanillapdf.tools <command> [options]\n\n");
    printf("Commands:\n");
    printf("  merge           Merge multiple PDF files\n");
    printf("  filter          Filter PDF content\n");
    printf("  extract         Extract PDF content\n");
    printf("  extract_object  Extract PDF objects\n");
    printf("  sign            Sign a PDF document\n");
    printf("  sign_custom     Sign a PDF with custom handler\n");
    printf("  verify          Verify PDF signatures\n");
    printf("  validate        Validate PDF file structure\n");
    printf("  encrypt         Encrypt a PDF document\n");
    printf("  decrypt         Decrypt a PDF document\n");
    printf("  read            Read a PDF using memory IO strategy\n");
    printf("  resave          Re-save a PDF document\n");
    printf("  write_custom    Write PDF with custom handler\n");
    printf("\nOptions:\n");
    printf("  --help, -h      Show this help message\n");
    printf("  --version, -v   Show version information\n");
}

int main(int argc, char *argv[]) {

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

    configure_openssl_modules_path(argv[0]);

    if (argc < 2) {
        print_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (0 == strcmp(argv[1], "--version") || 0 == strcmp(argv[1], "-v")) {
        print_version();
        return VANILLAPDF_TOOLS_ERROR_SUCCESS;
    }

    if (0 == strcmp(argv[1], "--help") || 0 == strcmp(argv[1], "-h")) {
        print_help();
        return VANILLAPDF_TOOLS_ERROR_SUCCESS;
    }

    if (0 == strcmp(argv[1], "merge")) {
        return process_merge(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "filter")) {
        return process_filter(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "extract")) {
        return process_extract(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "extract_object")) {
        return process_extract_object(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "sign")) {
        return process_sign(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "sign_custom")) {
        return process_sign_custom(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "verify")) {
        return process_verify(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "validate")) {
        return process_validate(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "encrypt")) {
        return process_encrypt(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "decrypt")) {
        return process_decrypt(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "read")) {
        return process_read(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "resave")) {
        return process_resave(argc - 2, &argv[2]);
    }

    if (0 == strcmp(argv[1], "write_custom")) {
        return process_write_custom(argc - 2, &argv[2]);
    }

    print_help();
    return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
}
