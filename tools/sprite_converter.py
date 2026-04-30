#!/usr/bin/env python3
"""
Sprite Converter for PocketSpirit
Converts PNG images to C arrays compatible with LVGL on ESP32.

Usage:
    python sprite_converter.py input.png --output sprite_data.h --name mySprite
    python sprite_converter.py input.png --output sprite_data.c --format binary
"""

import argparse
import os
import sys
from PIL import Image

def png_to_c_array(image_path, var_name, swap_bytes=True):
    """Convert a PNG image to a C array of uint16_t (RGB565)."""
    img = Image.open(image_path).convert("RGBA")
    width, height = img.size
    pixels = list(img.getdata())

    c_lines = []
    c_lines.append(f"// Generated from: {os.path.basename(image_path)}")
    c_lines.append(f"// Size: {width}x{height}")
    c_lines.append(f"const uint16_t {var_name}_map[] = {{")

    for y in range(height):
        row_vals = []
        for x in range(width):
            r, g, b, a = pixels[y * width + x]
            # Convert RGBA to RGB565
            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F
            b5 = (b >> 3) & 0x1F
            rgb565 = (r5 << 11) | (g6 << 5) | b5

            # Byte swap for SPI (ESP32 displays expect swapped)
            if swap_bytes:
                rgb565 = ((rgb565 & 0xFF) << 8) | ((rgb565 >> 8) & 0xFF)

            row_vals.append(f"0x{rgb565:04X}")
        c_lines.append("    " + ", ".join(row_vals) + ",")

    c_lines.append("};")
    c_lines.append(f"const uint16_t {var_name}_width = {width};")
    c_lines.append(f"const uint16_t {var_name}_height = {height};")

    return "\n".join(c_lines)


def png_to_binary(image_path, output_path, swap_bytes=True):
    """Convert a PNG image to a raw RGB565 binary file."""
    img = Image.open(image_path).convert("RGBA")
    width, height = img.size
    pixels = list(img.getdata())

    with open(output_path, "wb") as f:
        for y in range(height):
            for x in range(width):
                r, g, b, a = pixels[y * width + x]
                r5 = (r >> 3) & 0x1F
                g6 = (g >> 2) & 0x3F
                b5 = (b >> 3) & 0x1F
                rgb565 = (r5 << 11) | (g6 << 5) | b5

                if swap_bytes:
                    rgb565 = ((rgb565 & 0xFF) << 8) | ((rgb565 >> 8) & 0xFF)

                f.write(rgb565.to_bytes(2, byteorder='big'))

    print(f"Wrote {width}x{height} raw RGB565 to {output_path}")


def main():
    parser = argparse.ArgumentParser(description="Convert PNG sprites for PocketSpirit/LVGL")
    parser.add_argument("input", help="Input PNG file")
    parser.add_argument("--output", "-o", required=True, help="Output file (.h, .c, or .bin)")
    parser.add_argument("--name", "-n", default="sprite", help="C variable name prefix")
    parser.add_argument("--no-swap", action="store_true", help="Don't byte-swap for SPI")
    parser.add_argument("--resize", help="Resize to WxH (e.g. 64x64)")

    args = parser.parse_args()

    swap = not args.no_swap

    if args.resize:
        w, h = map(int, args.resize.split("x"))
        img = Image.open(args.input)
        resized_path = args.input.replace(".png", f"_{w}x{h}.png")
        img.resize((w, h), Image.LANCZOS).save(resized_path)
        args.input = resized_path

    ext = os.path.splitext(args.output)[1].lower()

    if ext == ".bin":
        png_to_binary(args.input, args.output, swap)
    elif ext in (".h", ".c"):
        result = png_to_c_array(args.input, args.name, swap)
        with open(args.output, "w") as f:
            f.write(result)
        print(f"Generated {args.output}")
    else:
        print(f"Unsupported output format: {ext}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
