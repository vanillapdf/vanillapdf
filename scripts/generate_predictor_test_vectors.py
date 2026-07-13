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

Paste the emitted arrays into ``src/vanillapdf.unittest/filter_test.cpp``.
Re-run this tool and replace the arrays whenever the geometry, sample data, or
filter selection changes.

Requires PyMuPDF (``pip install pymupdf``).

Examples::

    # Default: RGB 4x3 image, Average + Paeth vectors (the regression targets)
    python scripts/generate_predictor_test_vectors.py

    # All five filters
    python scripts/generate_predictor_test_vectors.py --filters all
"""

from __future__ import annotations

import argparse
import random
import zlib
from dataclasses import dataclass
from typing import Callable, Dict, List, Sequence

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

# PDF "PNG optimal" predictor: every scanline declares its own filter in its
# first byte, so a single value covers all five filter types (PDF 32000-1, 7.4.4.4).
PNG_OPTIMAL_PREDICTOR = 15
BITS_PER_BYTE = 8
ZLIB_COMPRESSION_LEVEL = 9
BITS_PER_COMPONENT_CHOICES = (1, 2, 4, 8, 16)

# Emitted C arrays are wrapped at this many bytes per line.
C_ARRAY_BYTES_PER_LINE = 12


def paeth_predictor(left: int, up: int, up_left: int) -> int:
    """Return the PNG PaethPredictor of the left, above and above-left bytes."""
    base = left + up - up_left
    p_left, p_up, p_up_left = abs(base - left), abs(base - up), abs(base - up_left)
    if p_left <= p_up and p_left <= p_up_left:
        return left
    if p_up <= p_up_left:
        return up
    return up_left


@dataclass(frozen=True)
class Geometry:
    """Image geometry, mirroring the FlateDecode DecodeParms of the same name."""

    colors: int
    bits_per_component: int
    columns: int

    @property
    def bytes_per_pixel(self) -> int:
        # Matches flate_decode_filter.cpp: colors * bits / 8 (integer division).
        return self.colors * self.bits_per_component // BITS_PER_BYTE

    @property
    def bytes_per_row(self) -> int:
        # Matches flate_decode_filter.cpp: (colors * columns * bits + 7) / 8.
        bits = self.colors * self.columns * self.bits_per_component
        return (bits + BITS_PER_BYTE - 1) // BITS_PER_BYTE


class PngFilterEncoder:
    """Forward PNG predictor codec: raw scanlines -> filtered scanlines.

    Forward filtering references the raw (not reconstructed) neighbours, so a
    scanline has no left-to-right dependency: the left and above-left rows can
    be pre-aligned and the whole row filtered in one pass. Only the encode
    direction lives here; the library implements decode, and the MuPDF oracle
    plus the unit test confirm that decode(encode(raw)) == raw.
    """

    def __init__(self, geometry: Geometry) -> None:
        self._geometry = geometry
        # Each entry maps a filter code to its per-byte delta from the raw
        # value; all share one signature so the encode loop stays uniform.
        self._filters: Dict[int, Callable[[int, int, int, int], int]] = {
            FILTER_CODES["none"]: lambda cur, left, up, up_left: cur,
            FILTER_CODES["sub"]: lambda cur, left, up, up_left: cur - left,
            FILTER_CODES["up"]: lambda cur, left, up, up_left: cur - up,
            FILTER_CODES["average"]: lambda cur, left, up, up_left: cur - (left + up) // 2,
            FILTER_CODES["paeth"]: lambda cur, left, up, up_left: cur - paeth_predictor(left, up, up_left),
        }

    def encode(self, raw_rows: Sequence[Sequence[int]], filter_code: int) -> bytes:
        """Filter every scanline, prefixing each with its ``filter_code`` byte."""
        apply_filter = self._filters[filter_code]
        bpp = self._geometry.bytes_per_pixel
        width = self._geometry.bytes_per_row

        def one_pixel_left(row: Sequence[int]) -> List[int]:
            # row shifted right by one pixel; the first pixel's neighbour is 0.
            return ([0] * bpp + list(row))[:width]

        out = bytearray()
        prior: Sequence[int] = [0] * width
        for row in raw_rows:
            left = one_pixel_left(row)
            up_left = one_pixel_left(prior)
            out.append(filter_code)
            out.extend(apply_filter(cur, lft, up, uplft) & 0xFF
                       for cur, lft, up, uplft in zip(row, left, prior, up_left))
            prior = row
        return bytes(out)


def build_raw_rows(geometry: Geometry, rows: int, seed: int) -> List[List[int]]:
    """Return a deterministic pseudo-random sample image of the given geometry."""
    rng = random.Random(seed)
    width = geometry.bytes_per_row
    return [[rng.randrange(256) for _ in range(width)] for _ in range(rows)]


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
        doc.update_object(
            xref,
            f"<< /Filter /FlateDecode /DecodeParms << /Predictor {PNG_OPTIMAL_PREDICTOR} "
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


def format_c_array(name: str, data: bytes) -> str:
    """Render ``data`` as a C ``static const unsigned char[]`` definition."""
    lines = [f"static const unsigned char {name}[] = {{"]
    for offset in range(0, len(data), C_ARRAY_BYTES_PER_LINE):
        chunk = ", ".join(f"0x{byte:02X}" for byte in data[offset:offset + C_ARRAY_BYTES_PER_LINE])
        lines.append(f"    {chunk},")
    lines.append("};")
    return "\n".join(lines)


def positive_int(value: str) -> int:
    """argparse ``type`` for a strictly positive integer."""
    parsed = int(value)
    if parsed < 1:
        raise argparse.ArgumentTypeError(f"must be a positive integer, got {value}")
    return parsed


def resolve_filters(spec: str) -> List[str]:
    """Parse a ``--filters`` value into an ordered list of filter names."""
    if spec.strip().lower() == "all":
        return list(FILTER_CODES)
    names = [name.strip().lower() for name in spec.split(",") if name.strip()]
    unknown = [name for name in names if name not in FILTER_CODES]
    if unknown:
        raise SystemExit(f"unknown filter(s): {', '.join(unknown)}")
    return names


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    """Parse and validate command-line arguments."""
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--colors", type=positive_int, default=3,
                        help="samples per pixel (/Colors, default 3)")
    parser.add_argument("--bits", type=int, default=8, dest="bits_per_component",
                        choices=BITS_PER_COMPONENT_CHOICES,
                        help="bits per component (/BitsPerComponent, default 8)")
    parser.add_argument("--columns", type=positive_int, default=4,
                        help="pixels per row (/Columns, default 4)")
    parser.add_argument("--rows", type=positive_int, default=3,
                        help="number of scanlines (default 3)")
    parser.add_argument("--filters", default="average,paeth",
                        help="comma-separated filter names or 'all' "
                             f"(choices: {', '.join(FILTER_CODES)}; default average,paeth)")
    parser.add_argument("--seed", type=int, default=0,
                        help="seed for the pseudo-random sample image (default 0)")
    return parser.parse_args(argv)


def render_vectors(geometry: Geometry, encoder: PngFilterEncoder,
                   raw_rows: Sequence[Sequence[int]], filter_names: Sequence[str]) -> str:
    """Encode, certify and render every requested filter as one C source block."""
    expected = bytes(byte for row in raw_rows for byte in row)

    blocks = []
    for name in filter_names:
        compressed = zlib.compress(encoder.encode(raw_rows, FILTER_CODES[name]), ZLIB_COMPRESSION_LEVEL)
        certify_vector(compressed, geometry, expected, name)
        blocks.append(format_c_array(f"INPUT_{name.upper()}", compressed))

    blocks.append(format_c_array("EXPECTED_RAW", expected))
    blocks.append(
        f"// geometry: Colors={geometry.colors} BitsPerComponent={geometry.bits_per_component} "
        f"Columns={geometry.columns} rows={len(raw_rows)} bytes_per_row={geometry.bytes_per_row}\n"
        "// verified against MuPDF (PyMuPDF) before emission")
    return "\n\n".join(blocks)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    geometry = Geometry(args.colors, args.bits_per_component, args.columns)
    encoder = PngFilterEncoder(geometry)
    raw_rows = build_raw_rows(geometry, args.rows, args.seed)
    print(render_vectors(geometry, encoder, raw_rows, resolve_filters(args.filters)))


if __name__ == "__main__":
    main()
