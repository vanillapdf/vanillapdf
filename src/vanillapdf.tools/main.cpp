#include "tools.h"

int main(int argc, char *argv[]) {

#if (defined(DEBUG) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO))
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(803506);
#endif /* DEBUG && COMPILER_MICROSOFT_VISUAL_STUDIO */

    std::string version = "vanillapdf.tools "
        + std::to_string(VANILLAPDF_TOOLS_VERSION_MAJOR) + "."
        + std::to_string(VANILLAPDF_TOOLS_VERSION_MINOR) + "."
        + std::to_string(VANILLAPDF_TOOLS_VERSION_PATCH);

    CLI::App app("Vanilla.PDF development and validation utility", "vanillapdf.tools");
    app.set_version_flag("-v,--version", version);
    app.require_subcommand(1);
    app.failure_message(CLI::FailureMessage::help);

    int exit_code = VANILLAPDF_TOOLS_ERROR_SUCCESS;

    register_merge(app, exit_code);
    register_filter(app, exit_code);
    register_extract(app, exit_code);
    register_extract_object(app, exit_code);
    register_sign(app, exit_code);
    register_sign_custom(app, exit_code);
    register_verify(app, exit_code);
    register_validate(app, exit_code);
    register_encrypt(app, exit_code);
    register_decrypt(app, exit_code);
    register_read(app, exit_code);
    register_resave(app, exit_code);
    register_remove_page(app, exit_code);
    register_write_custom(app, exit_code);
    register_generate(app, exit_code);

    try {
        app.parse(argc, argv);
    } catch (const CLI::Success& e) {
        // --help and --version
        return app.exit(e);
    } catch (const CLI::ParseError& e) {
        app.exit(e);
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    return exit_code;
}
