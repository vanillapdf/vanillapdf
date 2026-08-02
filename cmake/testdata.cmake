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
    URL      https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.0/corpus.tar.gz
    URL_HASH SHA256=6b20b3d5548dbc59793cd2364286049b97b27d14e191fa548848ef58f5c0769d
)
FetchContent_MakeAvailable(vanillapdf_testdata)

# The manifest is downloaded next to the extracted corpus/ so it is co-located
# with the fixtures (consumers can find it relative to the testdata root).
file(DOWNLOAD
    https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.0/manifest.json
    "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    EXPECTED_HASH SHA256=582220322130ed8a2cca106537900895576079f838eff67f71eb71c5764420ca
)

set(VANILLAPDF_TESTDATA_ROOT "${vanillapdf_testdata_SOURCE_DIR}"
    CACHE INTERNAL "Root the test manifest paths resolve against")
set(VANILLAPDF_CORPUS_DIR "${vanillapdf_testdata_SOURCE_DIR}/corpus"
    CACHE INTERNAL "Extracted test corpus directory")
set(VANILLAPDF_MANIFEST_FILE "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    CACHE INTERNAL "Downloaded test manifest")
