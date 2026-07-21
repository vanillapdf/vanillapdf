#!/usr/bin/env python3
"""Generate redistribution-safe sample PDF fixtures with ReportLab.

Produces feature-rich test documents (text in several standard fonts, an
underline, a procedurally generated image, and internal + external link
annotations) that vanillapdf can parse. Everything here is authored: only the
14 standard PDF fonts (referenced by metrics, NOT embedded) and Pillow-drawn
images are used, so there is no third-party font or image licensing to worry
about. The output PDFs are committed under fixtures/; this script exists to
reproduce them.

Requires: reportlab, pillow  (dev-time only; the fixtures themselves are
self-contained).

Usage: python scripts/generate_sample_documents.py [output_dir]
Default output_dir: fixtures/
"""

import io
import os
import sys

# vanillapdf's writer supports FlateDecode but not ASCII85 *encoding*, so emit
# binary Flate streams (no ASCII85 wrapping) to keep the fixture round-trippable.
import reportlab.rl_config
reportlab.rl_config.useA85 = 0

from reportlab.lib.pagesizes import letter
from reportlab.lib.units import inch
from reportlab.pdfgen import canvas
from reportlab.lib.utils import ImageReader
from PIL import Image, ImageDraw


def make_procedural_image(width=260, height=170):
    # A simple gradient with a couple of shapes — drawn here, so it is ours.
    image = Image.new("RGB", (width, height), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    for y in range(height):
        t = y / height
        draw.line(
            [(0, y), (width, y)],
            fill=(int(50 + 150 * t), int(130 + 70 * (1 - t)), int(210 - 120 * t)),
        )
    draw.ellipse([28, 30, 128, 130], outline=(255, 255, 255), width=5)
    draw.rectangle([140, 45, 230, 125], outline=(25, 25, 25), width=4)
    draw.line([28, 150, 230, 20], fill=(255, 255, 255), width=3)
    return image


def build_document(path):
    pdf = canvas.Canvas(path, pagesize=letter)
    page_width, page_height = letter
    left = inch

    # --- Page 1 -------------------------------------------------------------
    pdf.setFont("Helvetica-Bold", 24)
    pdf.drawString(left, page_height - 1.2 * inch, "Vanilla.PDF Sample Document")

    pdf.setFont("Helvetica", 11)
    pdf.drawString(left, page_height - 1.6 * inch,
                   "A synthetic, redistribution-safe test fixture generated with ReportLab.")

    # Underlined heading (draw the rule under the measured text width)
    heading = "Feature Showcase"
    pdf.setFont("Times-Bold", 16)
    heading_y = page_height - 2.2 * inch
    pdf.drawString(left, heading_y, heading)
    heading_width = pdf.stringWidth(heading, "Times-Bold", 16)
    pdf.line(left, heading_y - 2, left + heading_width, heading_y - 2)

    # A few standard fonts
    samples = [
        ("Times-Roman", "Times-Roman: The quick brown fox jumps over the lazy dog."),
        ("Courier", "Courier: The quick brown fox jumps over the lazy dog."),
        ("Helvetica-Oblique", "Helvetica-Oblique: The quick brown fox jumps over the lazy dog."),
    ]
    y = page_height - 2.7 * inch
    for font_name, text in samples:
        pdf.setFont(font_name, 12)
        pdf.drawString(left, y, text)
        y -= 0.3 * inch

    # Procedural image (embedded as PNG)
    buffer = io.BytesIO()
    make_procedural_image().save(buffer, format="PNG")
    buffer.seek(0)
    pdf.drawImage(ImageReader(buffer), left, page_height - 5.6 * inch,
                  width=2.6 * inch, height=1.7 * inch)

    # External link
    pdf.setFont("Helvetica", 12)
    external_text = "Open https://vanillapdf.com/"
    external_y = page_height - 6.1 * inch
    pdf.drawString(left, external_y, external_text)
    external_width = pdf.stringWidth(external_text, "Helvetica", 12)
    pdf.linkURL("https://vanillapdf.com/",
                (left, external_y - 2, left + external_width, external_y + 12),
                relative=0)

    # Internal link to page 2
    internal_text = "Jump to page 2 (internal link)"
    internal_y = page_height - 6.5 * inch
    pdf.drawString(left, internal_y, internal_text)
    internal_width = pdf.stringWidth(internal_text, "Helvetica", 12)
    pdf.linkAbsolute("", "page_two",
                     (left, internal_y - 2, left + internal_width, internal_y + 12))

    # A data table: many line + text operators, so page 1 carries a content
    # stream with a meaningful instruction count for the content-stream benchmark.
    rows, cols = 22, 6
    cell_w, cell_h = 0.9 * inch, 0.22 * inch
    table_top = page_height - 7.0 * inch
    pdf.setFont("Helvetica", 8)
    for r in range(rows + 1):
        line_y = table_top - r * cell_h
        pdf.line(left, line_y, left + cols * cell_w, line_y)
    for c in range(cols + 1):
        line_x = left + c * cell_w
        pdf.line(line_x, table_top, line_x, table_top - rows * cell_h)
    for r in range(rows):
        for c in range(cols):
            pdf.drawString(left + c * cell_w + 3, table_top - (r + 1) * cell_h + 6,
                           "R{}C{}".format(r + 1, c + 1))

    pdf.showPage()

    # --- Page 2 -------------------------------------------------------------
    pdf.bookmarkPage("page_two")
    pdf.setFont("Helvetica-Bold", 20)
    pdf.drawString(left, page_height - 1.2 * inch, "Page Two")

    pdf.setFont("Helvetica", 11)
    paragraph = [
        "This page carries additional text so the content stream has a meaningful",
        "instruction count for the content-stream benchmark. Lorem ipsum dolor sit",
        "amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore",
        "et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation.",
    ]
    y = page_height - 1.7 * inch
    for line in paragraph:
        pdf.drawString(left, y, line)
        y -= 0.28 * inch

    pdf.save()


def main():
    output_dir = sys.argv[1] if len(sys.argv) > 1 else "fixtures"
    output_path = os.path.join(output_dir, "sample-document.pdf")
    build_document(output_path)
    print("wrote {} ({} bytes)".format(output_path, os.path.getsize(output_path)))


if __name__ == "__main__":
    main()
