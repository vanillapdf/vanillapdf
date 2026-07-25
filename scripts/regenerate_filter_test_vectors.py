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
RGB8_ARGUMENTS = [
    "--colors", "3",
    "--bits", "8",
    "--columns", "4",
    "--rows", "3",
    "--seed", "0",
    "--compression-level", "9",
    "--bytes-per-line", "12",
]

# Sub-byte pixel depth: 3 colors x 4 bits = 12 pixel bits, so bytes-per-pixel is
# ceil(12/8)=2 where the old floor gave 1 -- the divergence that silently
# corrupted such streams (https://github.com/vanillapdf/vanillapdf/issues/443).
# Emitted with a SUBBYTE_ name prefix so the arrays don't collide with the RGB8
# set above.
SUBBYTE_ARGUMENTS = [
    "--colors", "3",
    "--bits", "4",
    "--columns", "4",
    "--rows", "3",
    "--seed", "0",
    "--compression-level", "9",
    "--bytes-per-line", "12",
    "--name-prefix", "SUBBYTE_",
]


if __name__ == "__main__":
    main(RGB8_ARGUMENTS)
    print()
    main(SUBBYTE_ARGUMENTS)
