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
    URL_HASH SHA256=51813b7279ae9c4ec3c261d158750e8174991f92a8d683dd178a3d458ef40551
)
FetchContent_MakeAvailable(vanillapdf_testdata)

# The manifest is downloaded next to the extracted corpus/ so it is co-located
# with the fixtures (consumers can find it relative to the testdata root).
file(DOWNLOAD
    https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.0/manifest.json
    "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    EXPECTED_HASH SHA256=1fff58b76678ef2c586da90a31c405669801b3122d045ce989555545c88991c5
)

set(VANILLAPDF_TESTDATA_ROOT "${vanillapdf_testdata_SOURCE_DIR}"
    CACHE INTERNAL "Root the test manifest paths resolve against")
set(VANILLAPDF_CORPUS_DIR "${vanillapdf_testdata_SOURCE_DIR}/corpus"
    CACHE INTERNAL "Extracted test corpus directory")
set(VANILLAPDF_MANIFEST_FILE "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    CACHE INTERNAL "Downloaded test manifest")
