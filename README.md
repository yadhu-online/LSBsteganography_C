# LSBsteganography_C
A command-line steganography tool that hides and extracts text data inside BMP images using the **Least Significant Bit (LSB)** technique.

---

## Overview

This project implements image-based steganography by embedding secret text data into the pixel data of a BMP image.  
The approach focuses on hiding the *existence* of communication rather than encrypting the content.

The application supports both **encoding** (hiding data) and **decoding** (extracting data) operations.

---

## Key Concepts Used

- LSB (Least Significant Bit) encoding  
- Bit manipulation  
- BMP file structure handling  
- File I/O in C  
- Command-line argument processing  

---

## Features

- Encode a `.txt` file into a `.bmp` image  
- Decode hidden data from a stego BMP image  
- Capacity validation before encoding  
- Magic string verification to detect steganographed images  
- Preserves original BMP header to avoid image corruption  

---

## Requirements

- Input image must be **BMP format**  
- Secret file must be a **text file (`.txt`)**  
- Image must have sufficient capacity  
- Command-line environment  

---

## Magic String

A predefined magic string is embedded during encoding to verify whether an image contains hidden data.
#define MAGIC_STRING "#*"

During decoding, this string is verified before extracting data.
Build Instructions

Compile the project using gcc:

gcc *.c -o steg

## Usage
Encoding

./steg -e <source file in .bmp> <secret file .txt> [output.bmp(optional)]

    If output.bmp is not provided, stego.bmp is created by default.

Decoding

./steg -d <encoded file in .bmp> [output_filename(optional)]

    If output filename is not provided, decode.txt is generated.

Encoding Workflow

    Validate command-line arguments

    Open source image and secret file

    Check image capacity

    Copy BMP header (54 bytes)

    Encode:

        Magic string

        Secret file extension size

        Secret file extension

        Secret file size

        Secret file data

    Copy remaining image data

Decoding Workflow

    Open stego image

    Verify magic string

    Decode:

        File extension size

        File extension

        Secret file size

        Secret data

    Reconstruct the original secret file

## Limitations

    Supports only BMP images

    Supports only text files as secret data

    No encryption applied to secret content

## Future Enhancements

    Support for additional image and file formats

    Password-based encryption

    GUI-based interface

    Improved payload capacity handling

## Author

Developed as a systems-level project to demonstrate steganography using C.
