# macros/VanillaPDFCheckStandardFeatures.cmake

include(CheckCXXSourceCompiles)

function(vanillapdf_check_standard_features TARGET)
    # Check std::from_chars for float/double
    set(_check_from_chars_float "
        #include <charconv>
        int main() {
            double d;
            auto result = std::from_chars(\"1.23\", \"1.23\" + 4, d);
            (void)result;
            return 0;
        }
    ")
    check_cxx_source_compiles("${_check_from_chars_float}" VANILLAPDF_HAS_FROM_CHARS_FLOAT)
    if(NOT VANILLAPDF_HAS_FROM_CHARS_FLOAT)
        target_compile_definitions(${TARGET} PRIVATE VANILLAPDF_NO_FROM_CHARS_FLOAT)
    endif()

    # Add more checks here as needed...
endfunction()
