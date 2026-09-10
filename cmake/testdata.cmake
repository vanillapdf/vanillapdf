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
    URL      https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.2/corpus.tar.gz
    URL_HASH SHA256=3ff282e2b559d2913c47ad89181da5883f893a9460447b2201edea3e67937747
)
FetchContent_MakeAvailable(vanillapdf_testdata)

# The manifest is downloaded next to the extracted corpus/ so it is co-located
# with the fixtures (consumers can find it relative to the testdata root).
file(DOWNLOAD
    https://github.com/vanillapdf/vanillapdf-testdata/releases/download/v1.2/manifest.json
    "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    EXPECTED_HASH SHA256=c20f4e2281c8f73ef48c1e19e724b6faa66ead51f329b45b9f856d834e7663c0
)

set(VANILLAPDF_TESTDATA_ROOT "${vanillapdf_testdata_SOURCE_DIR}"
    CACHE INTERNAL "Root the test manifest paths resolve against")
set(VANILLAPDF_CORPUS_DIR "${vanillapdf_testdata_SOURCE_DIR}/corpus"
    CACHE INTERNAL "Extracted test corpus directory")
set(VANILLAPDF_MANIFEST_FILE "${vanillapdf_testdata_SOURCE_DIR}/manifest.json"
    CACHE INTERNAL "Downloaded test manifest")
