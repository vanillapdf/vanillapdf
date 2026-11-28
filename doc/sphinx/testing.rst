Testing
=======

This page describes the test suite of the library, its test data, and the
fuzzing, conformance and coverage checks. Building the tests is described in
:doc:`building`.

Running tests
-------------

Tests require ``-DVANILLAPDF_ENABLE_TESTS=ON`` at configure time, which every
preset sets, and Python 3, which runs the corpus tests. The first configure
also downloads the :ref:`test data <test-data>`. After building, run the test
suite:

.. code-block:: bash

   ctest --preset <your-preset> --output-on-failure

The suite has three kinds of tests:

- **Unit tests** — the GoogleTest cases of ``vanillapdf.unittest``, named
  ``Suite.Case``
- **Corpus tests** — one test for each document of the test data, named after
  the file, for example ``AdobeXMLFormsSamples.pdf``
- **Tools tests** — the commands of ``vanillapdf.tools``, named ``Tools.*``

Run one kind with a name filter:

.. code-block:: bash

   ctest --preset <your-preset> -R "\.(pdf|PDF)$"    # Corpus tests
   ctest --preset <your-preset> -R "^Tools\."        # Tools tests
   ctest --preset <your-preset> -E "\.(pdf|PDF)$|^Tools\."    # Unit tests

Benchmarks are not part of the test suite. Build them with
``-DVANILLAPDF_ENABLE_BENCHMARK=ON`` and run the ``vanillapdf.benchmark``
executable.

**Windows (Visual Studio) note:** Visual Studio is a multi-config generator,
so you must specify a build configuration:

.. code-block:: bash

   ctest --preset windows-x64-msvc-18 --build-config Debug -R "^Tools\."
   ctest --preset windows-x64-msvc-18 --build-config Release

Documentation examples
----------------------

The programs shown in this documentation are not part of the test suite. They
are built the way a user builds them: ``examples/documentation-integration``
is a separate CMake project that builds every example from its own
``CMakeLists.txt`` against an installed library, and runs them as
``Examples.*`` tests. The ``Examples Integration`` workflow runs it on Linux,
Windows and macOS against the library installed by the vcpkg port. To run it
locally, install the port first and pass the vcpkg toolchain file:

.. code-block:: bash

   ./external/vcpkg/vcpkg install vanillapdf --head --overlay-ports=./ports
   cd examples/documentation-integration
   cmake --preset linux-x64-debug -DCMAKE_TOOLCHAIN_FILE=../../external/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build --preset linux-x64-debug
   ctest --preset linux-x64-debug

.. _test-data:

Test data
---------

The PDF documents the tests run on are not part of this repository. They live
in `vanillapdf-testdata <https://github.com/vanillapdf/vanillapdf-testdata>`_,
so a clone of the library stays small.

When tests are enabled, the configure step downloads a pinned release of the
corpus (``corpus.tar.gz``, about 50 MB) and its ``manifest.json``, checks their
SHA-256 checksums and extracts them into the build directory. The manifest
lists every document with its expected behavior, such as its password, the
checks to skip, or whether it is signed or merged. The first configure
therefore needs network access; later configures reuse the download.

To add a test document, add the PDF and its manifest entry to
vanillapdf-testdata and publish a new release of it. Then update the release
URLs and checksums in ``cmake/testdata.cmake``. The tests are created from the
manifest at configure time, so no CMake list needs to change.

The repository's README describes its layout and the fields of the manifest,
and ``SOURCES.md`` records where each document comes from. The documents keep
their own licenses: the Apache 2.0 license of this library does not cover them.
The same corpus is used by the language bindings, so a change to the manifest
affects their tests too.

.. _fuzzing:

Fuzzing
-------

Vanilla.PDF includes libFuzzer-based fuzz targets for the parser, content
streams, and decompression filters (ASCII85, ASCIIHex, RunLength, Flate with
predictors, DCT, JPX). Sources live in ``src/vanillapdf.fuzzer/``, with a seed
corpus for each target in ``src/vanillapdf.fuzzer/corpus/``. Fuzzing requires
Clang and ``-DVANILLAPDF_ENABLE_FUZZING=ON``:

.. code-block:: bash

   cmake --preset linux-x64-clang -DCMAKE_BUILD_TYPE=Release \
     -DVANILLAPDF_ENABLE_FUZZING=ON -DBUILD_SHARED_LIBS=OFF
   cmake --build --preset linux-x64-clang --target vanillapdf.fuzz.file_parse
   ./build/linux-x64-clang/src/vanillapdf.fuzzer/vanillapdf.fuzz.file_parse \
     src/vanillapdf.fuzzer/corpus/file_parse/

External conformance check
--------------------------

Vanilla.PDF re-saves selected files of its test corpus and checks the output
with `qpdf <https://qpdf.readthedocs.io/>`_ (``qpdf --check``) to catch
malformed output. The script lives at
``scripts/conformance_check.py`` and runs automatically in CI via
``.github/workflows/conformance-check.yml``. To run locally:

.. code-block:: bash

   python scripts/conformance_check.py --config scripts/conformance_check.cfg

Code coverage
-------------

Coverage of ``main`` and of every pull request that changes the code is
published on `Codecov <https://app.codecov.io/gh/vanillapdf/vanillapdf>`_, so a
local run is only needed to look at code that is not in a pull request yet.

To measure it locally, configure a Debug build with GCC or Clang. Optimized
builds inline and merge code, which makes the line counts unreliable:

.. code-block:: bash

   cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
   cmake --build --preset linux-x64-gcc
   ctest --preset linux-x64-gcc
   gcovr -r . --exclude-throw-branches --exclude-unreachable-branches \
     --html-details -o coverage/index.html

Open ``coverage/index.html`` in a browser for a report of every source file
with the covered lines marked. Only the library is instrumented, so the report
shows how much of the library the tests exercise. The two ``--exclude`` options
leave out branches no test can take, as the coverage workflow does, so the
numbers match Codecov.
