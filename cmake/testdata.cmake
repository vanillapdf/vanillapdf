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
    URL      https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.3/corpus.tar.gz
    URL_HASH SHA256=8d5998e9ed5fabd99ef6b968165a0be5829f8d2f98411675b58b47da89f81ac5
)
FetchContent_MakeAvailable(vanillapdf_testdata)

# The manifest is downloaded next to the extracted corpus/ so it is co-located
# with the fixtures (consumers can find it relative to the testdata root).
file(DOWNLOAD
    https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.3/manifest.json
    "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    EXPECTED_HASH SHA256=29a62fb2525f3d45a32a1f882da1d09022ac584061cf29716d7bbf969abee725
)

set(VANILLAPDF_TESTDATA_ROOT "${vanillapdf_testdata_SOURCE_DIR}"
    CACHE INTERNAL "Root the test manifest paths resolve against")
set(VANILLAPDF_CORPUS_DIR "${vanillapdf_testdata_SOURCE_DIR}/corpus"
    CACHE INTERNAL "Extracted test corpus directory")
set(VANILLAPDF_MANIFEST_FILE "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    CACHE INTERNAL "Downloaded test manifest")
