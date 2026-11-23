cmake_minimum_required(VERSION 3.20)

# This project builds and runs the examples shown in the documentation the way
# a user builds them: each example is its own CMake project that finds the
# installed vanillapdf package. It does not care where the package comes from;
# the caller makes it findable, for example with the toolchain file of the
# vcpkg instance that installed it, or with CMAKE_PREFIX_PATH.
#
# Against the vcpkg port overlay, for --head mode testing (uses HEAD_REF main):
#   cd <repo-root>
#   ./external/vcpkg/vcpkg install vanillapdf --head --overlay-ports=./ports
#   cd examples/documentation-integration
#   cmake --preset linux-x64-debug -DCMAKE_TOOLCHAIN_FILE=../../external/vcpkg/scripts/buildsystems/vcpkg.cmake
#   cmake --build --preset linux-x64-debug
#   ctest --preset linux-x64-debug

project(DocumentationIntegrationTest C CXX)

enable_testing()

set(EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../fixtures")

# Each example is built from its own CMakeLists.txt, unchanged, as the
# documentation tells the user to build it. The examples write their output
# into the working directory.
function(add_documentation_example EXAMPLE_NAME)
    add_subdirectory("${EXAMPLES_DIR}/${EXAMPLE_NAME}" "${EXAMPLE_NAME}")

    add_test(
        NAME Examples.${EXAMPLE_NAME}
        COMMAND ${EXAMPLE_NAME} ${ARGN}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endfunction()

add_documentation_example(quickstart)
add_documentation_example(create)

add_documentation_example(merge
    "example-merged.pdf"
    "${FIXTURES_DIR}/sample-document.pdf"
    "${FIXTURES_DIR}/nested-pages.pdf"
)

# Inspects the merged document, which shows that the merge kept every page
add_documentation_example(inspect "example-merged.pdf")

add_documentation_example(validation "${FIXTURES_DIR}/sample-document.pdf")

# The vcpkg port always enables encryption, which signing and encryption need
add_documentation_example(signing
    "${FIXTURES_DIR}/sample-document.pdf"
    "example-signed.pdf"
    "${FIXTURES_DIR}/certificates/Test_EC384_SHA512.pfx"
    "test"
)

# Verifies the document signed by the signing example
add_documentation_example(verification "example-signed.pdf")

add_documentation_example(visible-signature
    "${FIXTURES_DIR}/sample-document.pdf"
    "example-visible-signed.pdf"
    "${FIXTURES_DIR}/certificates/Test_EC384_SHA512.pfx"
    "test"
)

add_documentation_example(encryption
    "${FIXTURES_DIR}/sample-document.pdf"
    "example-encrypted.pdf"
    "user"
    "owner"
)

set_tests_properties(Examples.merge PROPERTIES FIXTURES_SETUP ExampleMerged)
set_tests_properties(Examples.inspect PROPERTIES
    FIXTURES_REQUIRED ExampleMerged
    PASS_REGULAR_EXPRESSION "Pages: 7"
)

set_tests_properties(Examples.signing PROPERTIES FIXTURES_SETUP ExampleSigned)

# The test certificate does not chain to a trusted root, so the example
# reports the signature as not valid; the signed content must be intact
set_tests_properties(Examples.verification PROPERTIES
    FIXTURES_REQUIRED ExampleSigned
    PASS_REGULAR_EXPRESSION "Document intact: yes"
)

# The verification example finds the one signature, in the visible field
set_tests_properties(Examples.visible-signature PROPERTIES FIXTURES_SETUP ExampleVisibleSigned)
add_test(
    NAME Examples.visible-signature.verification
    COMMAND verification "example-visible-signed.pdf"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
set_tests_properties(Examples.visible-signature.verification PROPERTIES
    FIXTURES_REQUIRED ExampleVisibleSigned
    PASS_REGULAR_EXPRESSION "Document intact: yes.*Verified 1 signature"
)
