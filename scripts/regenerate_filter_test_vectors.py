#!/usr/bin/env python3
"""Reproduce the FlateDecode predictor vectors committed in filter_test.cpp.

``png_predictor_vectors.py`` is the general, parametrised tool. This wrapper
pins the exact parameters that produced the ``INPUT_*`` / ``EXPECTED_RAW`` arrays
in ``src/vanillapdf.unittest/filter_test.cpp``, so running it always reproduces
those bytes regardless of the tool's default values. Paste its output over the
arrays in that file whenever they need to be regenerated.

Usage::

    python scripts/regenerate_filter_test_vectors.py
"""

from png_predictor_vectors import main

# Exact parameters behind the committed vectors: a 4x3 RGB (8-bit) image with a
# fixed random seed, zlib level 9, wrapped at 12 bytes per emitted C line.
PINNED_ARGUMENTS = [
    "--colors", "3",
    "--bits", "8",
    "--columns", "4",
    "--rows", "3",
    "--seed", "0",
    "--compression-level", "9",
    "--bytes-per-line", "12",
]


if __name__ == "__main__":
    main(PINNED_ARGUMENTS)
