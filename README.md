# Huff - Text File Compressor

## Project Description
CLI program that utilizes the Huffman Coding to perform lossless data compression and decompression in text files.

## Screenshots
![screenshot1](https://raw.github.com/fcolona/Huff/main/screenshot1.svg)
![screenshot2](https://raw.github.com/fcolona/Huff/main/screenshot2.svg)

## Main Features
- Lossless compression
- Easy to use
- Efficient
- Worldwide language support
- Multiple file formats support

## Main Technologies:
- C++
- STL
- Boost

## Benchmark
Having tested compressing parts of books in english and portuguese (each part varying from 1000 bytes to 12000 bytes), the program had an average rate of reduction of 40.7% in the size of the files.

## How to Set up
### On Arch-based distros
- Download the pre-compiled binary from the releases tab

### On any other distro
- Clone the repository  
    - ```git clone https://github.com/fcolona/Text-File-Compressor```
- Enter the project directory
    - ```cd Text-File-Compressor```
- Download Boost
    - ```curl --output boost_1_87_0.tar.gz https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.bz2```
- Extract the files
    - ```tar -xf boost_1_87_0.tar.gz```
- Move the Boost folder to the includes directory
    - ```mv boost_1_87_0.tar.gz includes```
- Run build script
    - ```make build```
- Copy the executable to the user binary folder 
    - ```cp build/huff /usr/bin```

## Try It Out!
- Expected compression command
    - ```huff [input file path] [compressed file path]```
- Expected decompression command
    - ```huff -d [compressed file path] [decompressed file path]```
- Example
    - ```huff text out/compressed_file```
    - ```huff -d compressed_file out/decompressed_file```