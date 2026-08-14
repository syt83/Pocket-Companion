# python convert_rgb565.py file.png file.rgb565 --width 160 --height 160 --bg 255,0,255

#!/usr/bin/env python3
"""
Pocket Companion RGB565 Converter

Converts PNG/JPG/JPEG images into the simple .rgb565 format
used by CharacterRenderer.

Output format:
    uint16 width   (little-endian)
    uint16 height  (little-endian)
    RGB565 pixels, row-major, 16-bit little-endian

Examples:
    python convert_rgb565.py happy.png
    python convert_rgb565.py happy.png happy.rgb565
    python convert_rgb565.py happy.png happy.rgb565 --width 200 --height 200
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("ERROR: Pillow is required.")
    print("Install it with: python -m pip install Pillow")
    sys.exit(1)


def rgb888_to_rgb565(r: int, g: int, b: int) -> int:
    """Convert 8-bit RGB channels to RGB565."""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def prepare_image(
    image: Image.Image,
    width: int,
    height: int,
    preserve_aspect: bool,
    background: tuple[int, int, int],
) -> Image.Image:
    """Resize an image to the requested size."""
    image = image.convert("RGBA")

    if preserve_aspect:
        scale = min(width / image.width, height / image.height)
        new_w = max(1, round(image.width * scale))
        new_h = max(1, round(image.height * scale))

        image = image.resize(
            (new_w, new_h),
            Image.Resampling.LANCZOS,
        )

        canvas = Image.new(
            "RGBA",
            (width, height),
            (*background, 255),
        )

        x = (width - new_w) // 2
        y = (height - new_h) // 2
        canvas.alpha_composite(image, (x, y))

        return canvas.convert("RGB")

    return image.resize(
        (width, height),
        Image.Resampling.LANCZOS,
    ).convert("RGB")


def write_rgb565(image: Image.Image, output_path: Path) -> None:
    """
    Write the custom Pocket Companion .rgb565 format.

    Header:
        uint16 width
        uint16 height

    Followed by:
        width * height RGB565 pixels, little-endian
    """
    image = image.convert("RGB")
    width, height = image.size

    with output_path.open("wb") as f:
        f.write(struct.pack("<HH", width, height))

        # Stream rows so we don't need a huge temporary pixel buffer.
        for y in range(height):
            row = image.crop((0, y, width, y + 1))
            pixels = row.getdata()

            row_data = bytearray(width * 2)
            offset = 0

            for r, g, b in pixels:
                value = rgb888_to_rgb565(r, g, b)
                struct.pack_into("<H", row_data, offset, value)
                offset += 2

            f.write(row_data)


def parse_rgb(value: str) -> tuple[int, int, int]:
    parts = [int(x.strip()) for x in value.split(",")]

    if len(parts) != 3 or any(x < 0 or x > 255 for x in parts):
        raise ValueError

    return parts[0], parts[1], parts[2]


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Convert PNG/JPG/JPEG to Pocket Companion .rgb565"
    )

    parser.add_argument(
        "input",
        type=Path,
        help="Input PNG/JPG/JPEG file",
    )

    parser.add_argument(
        "output",
        type=Path,
        nargs="?",
        help="Output .rgb565 file; defaults to input name with .rgb565",
    )

    parser.add_argument(
        "--width",
        type=int,
        default=200,
        help="Output width (default: 200)",
    )

    parser.add_argument(
        "--height",
        type=int,
        default=200,
        help="Output height (default: 200)",
    )

    parser.add_argument(
        "--stretch",
        action="store_true",
        help="Stretch image instead of preserving aspect ratio",
    )

    parser.add_argument(
        "--bg",
        default="255,0,255",
        help="Background RGB for transparent images (default: 255,0,255)",
    )

    args = parser.parse_args()

    if not args.input.exists():
        print(f"ERROR: Input file not found: {args.input}")
        return 1

    if args.width <= 0 or args.height <= 0:
        print("ERROR: width and height must be positive.")
        return 1

    output = args.output or args.input.with_suffix(".rgb565")

    if output.suffix.lower() != ".rgb565":
        print("ERROR: Output file must have a .rgb565 extension.")
        return 1

    try:
        background = parse_rgb(args.bg)
    except (ValueError, TypeError):
        print("ERROR: --bg must be R,G,B with values from 0 to 255.")
        return 1

    try:
        with Image.open(args.input) as source:
            print(f"Input : {args.input}")
            print(f"Source: {source.width} x {source.height}")
            print(f"Mode  : {source.mode}")

            prepared = prepare_image(
                source,
                args.width,
                args.height,
                preserve_aspect=not args.stretch,
                background=background,
            )

        write_rgb565(prepared, output)

    except OSError as exc:
        print(f"ERROR: image processing failed: {exc}")
        return 1

    output_size = 4 + (args.width * args.height * 2)

    print(f"Output: {output}")
    print(f"Size  : {args.width} x {args.height}")
    print("Format: RGB565 little-endian")
    print(f"Bytes : {output_size}")
    print("Done.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())