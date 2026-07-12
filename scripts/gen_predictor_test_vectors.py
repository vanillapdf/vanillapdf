#!/usr/bin/env python3
"""Generate PNG-predictor regression test vectors for the FlateDecode filter.

When a PDF FlateDecode stream carries DecodeParms with ``Predictor >= 10`` the
filter reconstructs each scanline using the PNG predictor algorithm
(https://www.w3.org/TR/PNG-Filters.html). PDF Predictor codes 10-15 are byte
for byte the PNG per-scanline filters, so a real PNG decoder is a valid,
independent oracle for the reconstruction.

This tool runs offline (never in CI). It:

  1. Forward-filters a known raw image with a chosen PNG filter type.
  2. Wraps the filtered scanlines into a spec-compliant PNG and decodes it with
     Pillow (libpng). This proves the filtered bytes truly reconstruct to the
     raw image using a decoder we did NOT write -- without it, the test would
     merely compare our Python encoder against our C++ decoder.
  3. Emits C arrays: the zlib-compressed filtered stream (the decoder input)
     and the raw image (the expected output).

Paste the emitted arrays into ``src/vanillapdf.unittest/filter_test.cpp``.
Re-run this tool and replace the arrays whenever the geometry, sample data, or
filter selection changes.

Requires Pillow (``pip install pillow``).

Examples::

    # Default: RGB 4x3 image, Average + Paeth vectors (the regression targets)
    python scripts/gen_predictor_test_vectors.py

    # All five filters, a reproducible random RGB image
    python scripts/gen_predictor_test_vectors.py --filters all --seed 42
"""

from __future__ import annotations

import argparse
import io
import random
import struct
import zlib
from dataclasses import dataclass
from typing import Callable, Dict, List, Sequence

try:
    from PIL import Image
except ImportError as exc:  # pragma: no cover - environment guard
    raise SystemExit("Pillow is required: pip install pillow") from exc

# PNG filter type codes (the leading byte of every encoded scanline).
FILTER_CODES: Dict[str, int] = {
    "none": 0,
    "sub": 1,
    "up": 2,
    "average": 3,
    "paeth": 4,
}

# Channel count -> (PNG color type, Pillow mode). Only the byte layout matters
# for the predictor oracle; the channel semantics are irrelevant.
PNG_COLOR_TYPES: Dict[int, int] = {
    1: 0,   # grayscale
    2: 4,   # grayscale + alpha
    3: 2,   # RGB
    4: 6,   # RGBA
}


def paeth_predictor(left: int, up: int, up_left: int) -> int:
    """The PNG PaethPredictor of the left, above and above-left bytes."""
    p = left + up - up_left
    p_left, p_up, p_up_left = abs(p - left), abs(p - up), abs(p - up_left)
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
        return self.colors * self.bits_per_component // 8

    @property
    def bytes_per_row(self) -> int:
        # Matches flate_decode_filter.cpp: (colors * columns * bits + 7) / 8.
        return (self.colors * self.columns * self.bits_per_component + 7) // 8


class PngFilterEncoder:
    """Forward PNG predictor codec: raw scanlines -> filtered scanlines.

    Only the encode direction lives here; the library implements decode, and the
    Pillow oracle plus the unit test confirm that decode(encode(raw)) == raw.
    """

    def __init__(self, geometry: Geometry) -> None:
        self._geometry = geometry
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

        out = bytearray()
        prior = [0] * width
        for row in raw_rows:
            out.append(filter_code)
            for x in range(width):
                left = row[x - bpp] if x >= bpp else 0
                up = prior[x]
                up_left = prior[x - bpp] if x >= bpp else 0
                out.append(apply_filter(row[x], left, up, up_left) & 0xFF)
            prior = list(row)
        return bytes(out)


def build_raw_rows(geometry: Geometry, rows: int, seed: int | None) -> List[List[int]]:
    """Produce a deterministic sample image: seeded random, or a fixed pattern."""
    width = geometry.bytes_per_row
    if seed is not None:
        rng = random.Random(seed)
        return [[rng.randrange(256) for _ in range(width)] for _ in range(rows)]

    # Fixed, human-readable fallback used by the checked-in vectors.
    base = [
        [0x10, 0x20, 0x30, 0x45, 0x51, 0x60, 0x7A, 0x81, 0x92, 0xA3, 0xB1, 0xC4],
        [0x11, 0x2F, 0x33, 0x40, 0x58, 0x66, 0x70, 0x88, 0x99, 0xA0, 0xBB, 0xC0],
        [0xF0, 0xE1, 0xD2, 0x03, 0x14, 0x25, 0x66, 0x57, 0x48, 0x99, 0x8A, 0x7B],
    ]
    return [[base[r % len(base)][x % len(base[0])] for x in range(width)]
            for r in range(rows)]


def _png_chunk(tag: bytes, data: bytes) -> bytes:
    crc = zlib.crc32(tag + data) & 0xFFFFFFFF
    return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", crc)


def wrap_png(geometry: Geometry, rows: int, compressed_scanlines: bytes) -> bytes:
    """Assemble a spec-compliant PNG whose IDAT is ``compressed_scanlines``."""
    if geometry.colors not in PNG_COLOR_TYPES:
        raise SystemExit(f"cannot map Colors={geometry.colors} to a PNG color type; "
                         "oracle validation supports 1-4 colors")
    color_type = PNG_COLOR_TYPES[geometry.colors]
    ihdr = struct.pack(
        ">IIBBBBB",
        geometry.columns, rows, geometry.bits_per_component, color_type, 0, 0, 0)
    return (b"\x89PNG\r\n\x1a\n"
            + _png_chunk(b"IHDR", ihdr)
            + _png_chunk(b"IDAT", compressed_scanlines)
            + _png_chunk(b"IEND", b""))


def verify_with_pillow(png_bytes: bytes, raw_rows: Sequence[Sequence[int]]) -> None:
    """Decode the PNG via libpng (Pillow) and assert it matches the raw image."""
    with Image.open(io.BytesIO(png_bytes)) as image:
        decoded = image.tobytes()
    expected = bytes(byte for row in raw_rows for byte in row)
    if decoded != expected:
        raise SystemExit("independent PNG oracle disagreed with the encoder; "
                         "the generated vectors are NOT trustworthy")


def format_c_array(name: str, data: bytes) -> str:
    lines = [f"static const unsigned char {name}[] = {{"]
    for offset in range(0, len(data), 12):
        chunk = ", ".join(f"0x{byte:02X}" for byte in data[offset:offset + 12])
        lines.append(f"    {chunk},")
    lines.append("};")
    return "\n".join(lines)


def resolve_filters(spec: str) -> List[str]:
    if spec.strip().lower() == "all":
        return list(FILTER_CODES)
    names = [name.strip().lower() for name in spec.split(",") if name.strip()]
    unknown = [name for name in names if name not in FILTER_CODES]
    if unknown:
        raise SystemExit(f"unknown filter(s): {', '.join(unknown)}")
    return names


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--colors", type=int, default=3,
                        help="samples per pixel (/Colors, 1-4, default 3)")
    parser.add_argument("--bits", type=int, default=8, dest="bits_per_component",
                        help="bits per component (/BitsPerComponent, default 8)")
    parser.add_argument("--columns", type=int, default=4,
                        help="pixels per row (/Columns, default 4)")
    parser.add_argument("--rows", type=int, default=3,
                        help="number of scanlines (default 3)")
    parser.add_argument("--filters", default="average,paeth",
                        help="comma-separated filter names or 'all' "
                             f"(choices: {', '.join(FILTER_CODES)}; default average,paeth)")
    parser.add_argument("--seed", type=int, default=None,
                        help="seed for random pixels; omit for the fixed sample image")
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    geometry = Geometry(args.colors, args.bits_per_component, args.columns)
    encoder = PngFilterEncoder(geometry)
    raw_rows = build_raw_rows(geometry, args.rows, args.seed)

    for name in resolve_filters(args.filters):
        filtered = encoder.encode(raw_rows, FILTER_CODES[name])
        compressed = zlib.compress(filtered, 9)

        # Independent oracle: a real PNG decoder must agree that the stream
        # reconstructs to the raw image before we trust the vector.
        verify_with_pillow(wrap_png(geometry, args.rows, compressed), raw_rows)

        print(format_c_array(f"INPUT_{name.upper()}", compressed))
        print()

    print(format_c_array("EXPECTED_RAW",
                          bytes(byte for row in raw_rows for byte in row)))
    print()
    print(f"// geometry: Colors={geometry.colors} "
          f"BitsPerComponent={geometry.bits_per_component} "
          f"Columns={geometry.columns} rows={args.rows} "
          f"bytes_per_row={geometry.bytes_per_row}")
    print("// verified against Pillow (libpng) before emission")


if __name__ == "__main__":
    main()
