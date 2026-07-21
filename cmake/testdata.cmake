# Fetches the vanillapdf-testdata corpus and manifest once, for every test
# target. The PDF fixtures live in a separate data repository so cloning this
# repo does not drag ~95 MB of binaries along. A pinned, checksum-verified
# archive is downloaded at configure time (the enumeration and the tools CLI
# tests need the files present now).
#
# Only corpus.tar.gz is fetched: the broken/ and analysis/ sets in that repo are
# analysis-only and are never executed as tests.
#
# Included from the top-level CMakeLists before the source subdirectories, so it
# runs ahead of both vanillapdf.tools and vanillapdf.test. Exports (cache-global):
#   VANILLAPDF_TESTDATA_ROOT - extract root; every manifest path resolves here
#   VANILLAPDF_CORPUS_DIR    - the corpus/ directory of PDF fixtures
#   VANILLAPDF_MANIFEST_FILE - the downloaded manifest.json (fixtures + expectations)

include(FetchContent)

FetchContent_Declare(vanillapdf_testdata
    URL      https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1/corpus.tar.gz
    URL_HASH SHA256=9c55d8f07cdcc811fc381ef06afd3802b06e72591ba10331c68730ce773ff6ee
)
FetchContent_MakeAvailable(vanillapdf_testdata)

file(DOWNLOAD
    https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1/manifest.json
    "${CMAKE_BINARY_DIR}/vanillapdf-testdata/manifest.json"
    EXPECTED_HASH SHA256=3476e8417a4b867ff0677c2104facab2717833395f6ae0264cf4edf6e48bb0c2
)

set(VANILLAPDF_TESTDATA_ROOT "${vanillapdf_testdata_SOURCE_DIR}"
    CACHE INTERNAL "Root the test manifest paths resolve against")
set(VANILLAPDF_CORPUS_DIR "${vanillapdf_testdata_SOURCE_DIR}/corpus"
    CACHE INTERNAL "Extracted test corpus directory")
set(VANILLAPDF_MANIFEST_FILE "${CMAKE_BINARY_DIR}/vanillapdf-testdata/manifest.json"
    CACHE INTERNAL "Downloaded test manifest")
