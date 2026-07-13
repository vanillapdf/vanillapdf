#!/usr/bin/env python3
"""Generate PNG-predictor regression test vectors for the FlateDecode filter.

When a PDF FlateDecode stream carries DecodeParms with ``Predictor >= 10`` the
filter reconstructs each scanline using the PNG predictor algorithm
(https://www.w3.org/TR/PNG-Filters.html).

This tool runs offline (never in CI). It:

  1. Forward-filters a known raw image with a chosen PNG filter type.
  2. Compresses the filtered scanlines and lets MuPDF (PyMuPDF) -- an
     independent PDF engine we did NOT write -- decode them back through a
     FlateDecode+Predictor stream, asserting it reconstructs the original
     image. MuPDF builds the PDF object from a declarative dictionary and the
     stream bytes (no hand-assembled container) and applies the exact PDF
     predictor under test. Crucially the check is byte-for-byte equality with
     the source image, not mere decodability: a wrong predictor still decodes
     cleanly, just to the wrong pixels, so equality is what certifies our
     forward filters (including Paeth) against a third-party implementation.
  3. Emits C arrays: the zlib-compressed filtered stream (the decoder input)
     and the raw image (the expected output).

Paste the emitted arrays into ``src/vanillapdf.unittest/filter_test.cpp``. To
reproduce the exact arrays committed there, run
``scripts/regenerate_filter_test_vectors.py``, which pins the parameters they
were generated with.

Sub-byte pixel depths (``colors * bits`` not a multiple of 8) are rejected:
the byte stride this tool mirrors from ``flate_decode_filter.cpp`` floors,
while the PNG specification (and MuPDF) round up, so such vectors can never
certify (see GitHub issue #443).

Requires PyMuPDF (``pip install pymupdf``).

Run with no arguments to print one vector per PNG filter type::

    python scripts/png_predictor_vectors.py
"""

from __future__ import annotations

import argparse
import random
import zlib
from dataclasses import dataclass
from typing import Dict, List, Sequence

try:
    import fitz  # PyMuPDF
except ImportError as exc:  # pragma: no cover - environment guard
    raise SystemExit("PyMuPDF is required: pip install pymupdf") from exc

# PNG filter type codes (the leading byte of every encoded scanline).
FILTER_CODES: Dict[str, int] = {
    "none": 0,
    "sub": 1,
    "up": 2,
    "average": 3,
    "paeth": 4,
}


def paeth_predictor(left: int, above: int, above_left: int) -> int:
    """Return the PNG Paeth prediction from the three neighbouring bytes.

    The predictor is whichever neighbour is closest to the linear estimate
    ``left + above - above_left``; ties prefer the left neighbour, then above.
    """
    estimate = left + above - above_left

    distance_to_left = abs(estimate - left)
    distance_to_above = abs(estimate - above)
    distance_to_above_left = abs(estimate - above_left)

    if distance_to_left <= distance_to_above and distance_to_left <= distance_to_above_left:
        return left
    if distance_to_above <= distance_to_above_left:
        return above
    return above_left


def png_filtered_byte(filter_code: int, current: int, left: int, above: int, above_left: int) -> int:
    """Return ``current`` minus the PNG prediction for the given filter.

    Every PNG filter encodes a byte as the raw value minus a prediction from
    already-known neighbours; the caller reduces the result modulo 256.
    """
    if filter_code == FILTER_CODES["none"]:
        return current
    if filter_code == FILTER_CODES["sub"]:
        return current - left
    if filter_code == FILTER_CODES["up"]:
        return current - above
    if filter_code == FILTER_CODES["average"]:
        return current - (left + above) // 2
    if filter_code == FILTER_CODES["paeth"]:
        return current - paeth_predictor(left, above, above_left)
    raise ValueError(f"unknown PNG filter code: {filter_code}")


@dataclass(frozen=True)
class Geometry:
    """Image geometry, mirroring the FlateDecode DecodeParms of the same name."""

    colors: int
    bits_per_component: int
    columns: int

    @property
    def bytes_per_pixel(self) -> int:
        # Matches flate_decode_filter.cpp: colors * bits / 8 (integer division).
        return self.colors * self.bits_per_component // 8

    @property
    def bytes_per_row(self) -> int:
        # Matches flate_decode_filter.cpp: (colors * columns * bits + 7) / 8.
        total_bits = self.colors * self.columns * self.bits_per_component
        return (total_bits + 7) // 8


def build_raw_rows(geometry: Geometry, rows: int, seed: int) -> List[List[int]]:
    """Return a deterministic pseudo-random sample image of the given geometry."""
    rng = random.Random(seed)
    width = geometry.bytes_per_row

    image = []
    for _ in range(rows):
        row = []
        for _ in range(width):
            row.append(rng.randrange(256))
        image.append(row)
    return image


def encode_scanlines(geometry: Geometry, raw_rows: Sequence[Sequence[int]], filter_code: int) -> bytes:
    """Forward-filter every scanline, prefixing each with its filter-code byte.

    Forward filtering reads the raw (not reconstructed) neighbours, so there is
    no left-to-right dependency within a row. Only encode lives here; the
    library decodes, and the MuPDF oracle plus the unit test confirm the round
    trip.
    """
    bytes_per_pixel = geometry.bytes_per_pixel
    width = geometry.bytes_per_row

    out = bytearray()
    previous_row: Sequence[int] = [0] * width
    for row in raw_rows:
        out.append(filter_code)
        for x in range(width):
            # Off-image neighbours (past the left edge) are treated as zero.
            if x >= bytes_per_pixel:
                left = row[x - bytes_per_pixel]
                above_left = previous_row[x - bytes_per_pixel]
            else:
                left = 0
                above_left = 0
            above = previous_row[x]

            filtered = png_filtered_byte(filter_code, row[x], left, above, above_left)
            out.append(filtered & 0xFF)
        previous_row = row
    return bytes(out)


def flatten_rows(raw_rows: Sequence[Sequence[int]]) -> bytes:
    """Concatenate the scanlines into one byte string (the expected image)."""
    flat = bytearray()
    for row in raw_rows:
        flat.extend(row)
    return bytes(flat)


def decode_with_pdf_engine(compressed: bytes, geometry: Geometry) -> bytes:
    """Decode a FlateDecode+Predictor stream with MuPDF (the independent oracle).

    Builds a one-object PDF whose stream is the compressed, predictor-filtered
    data and lets MuPDF apply the FlateDecode filter and PNG predictor, exactly
    as a PDF reader would. Returns the reconstructed raw image bytes.
    """
    doc = fitz.open()
    try:
        xref = doc.get_new_xref()
        # An empty dict must exist before a stream can be attached; the stream
        # is stored verbatim (compress=False) and the filter is declared after.
        doc.update_object(xref, "<< >>")
        doc.update_stream(xref, compressed, new=True, compress=False)
        # Predictor 15 = PNG "optimal": each scanline carries its own filter byte,
        # which is what the encoder emits, so it is fixed rather than configurable.
        doc.update_object(
            xref,
            "<< /Filter /FlateDecode /DecodeParms << /Predictor 15 "
            f"/Colors {geometry.colors} /BitsPerComponent {geometry.bits_per_component} "
            f"/Columns {geometry.columns} >> /Length {len(compressed)} >>")
        return doc.xref_stream(xref)
    finally:
        doc.close()


def certify_vector(compressed: bytes, geometry: Geometry, expected: bytes, filter_name: str) -> None:
    """Abort unless MuPDF reconstructs ``expected`` from the compressed stream."""
    if decode_with_pdf_engine(compressed, geometry) != expected:
        raise SystemExit(f"MuPDF disagreed on the '{filter_name}' filter; "
                         "the generated vector is NOT trustworthy")


def format_c_array(name: str, data: bytes, bytes_per_line: int) -> str:
    """Render ``data`` as a C ``static const unsigned char[]`` definition."""
    lines = []
    lines.append("static const unsigned char " + name + "[] = {")
    for start in range(0, len(data), bytes_per_line):
        hex_values = []
        for byte in data[start:start + bytes_per_line]:
            hex_values.append(f"0x{byte:02X}")
        lines.append("    " + ", ".join(hex_values) + ",")
    lines.append("};")
    return "\n".join(lines)


def positive_int(value: str) -> int:
    """argparse ``type`` for a strictly positive integer."""
    parsed = int(value)
    if parsed < 1:
        raise argparse.ArgumentTypeError(f"must be a positive integer, got {value!r}")
    return parsed


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    """Parse and validate command-line arguments."""
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--colors", type=positive_int, default=3,
                        help="samples per pixel (/Colors, default 3)")
    parser.add_argument("--bits", type=int, default=8, dest="bits_per_component",
                        choices=(1, 2, 4, 8, 16),
                        help="bits per component (/BitsPerComponent, default 8)")
    parser.add_argument("--columns", type=positive_int, default=4,
                        help="pixels per row (/Columns, default 4)")
    parser.add_argument("--rows", type=positive_int, default=3,
                        help="number of scanlines (default 3)")
    parser.add_argument("--seed", type=int, default=0,
                        help="seed for the pseudo-random sample image (default 0)")
    parser.add_argument("--compression-level", type=int, default=9, choices=range(0, 10),
                        dest="compression_level",
                        help="zlib compression level (default 9)")
    parser.add_argument("--bytes-per-line", type=positive_int, default=12,
                        dest="bytes_per_line",
                        help="bytes per line in the emitted C arrays (default 12)")

    args = parser.parse_args(argv)
    if (args.colors * args.bits_per_component) % 8 != 0:
        parser.error("colors * bits must be a multiple of 8 (whole-byte pixels); "
                     "sub-byte pixel depths cannot certify against MuPDF "
                     "(see GitHub issue #443)")
    return args


def render_vectors(geometry: Geometry, raw_rows: Sequence[Sequence[int]],
                   compression_level: int, bytes_per_line: int) -> str:
    """Encode, certify and render one vector per PNG filter as a C source block."""
    expected = flatten_rows(raw_rows)

    blocks = []
    for name, filter_code in FILTER_CODES.items():
        filtered = encode_scanlines(geometry, raw_rows, filter_code)
        compressed = zlib.compress(filtered, compression_level)
        certify_vector(compressed, geometry, expected, name)
        blocks.append(format_c_array("INPUT_" + name.upper(), compressed, bytes_per_line))

    blocks.append(format_c_array("EXPECTED_RAW", expected, bytes_per_line))

    geometry_comment = (
        f"// geometry: Colors={geometry.colors} "
        f"BitsPerComponent={geometry.bits_per_component} "
        f"Columns={geometry.columns} rows={len(raw_rows)} "
        f"bytes_per_row={geometry.bytes_per_row}\n"
        "// verified against MuPDF (PyMuPDF) before emission")
    blocks.append(geometry_comment)
    return "\n\n".join(blocks)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    geometry = Geometry(args.colors, args.bits_per_component, args.columns)
    raw_rows = build_raw_rows(geometry, args.rows, args.seed)
    output = render_vectors(geometry, raw_rows, args.compression_level, args.bytes_per_line)
    print(output)


if __name__ == "__main__":
    main()
