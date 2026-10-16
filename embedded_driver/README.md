# Embedded Driver

## Directory Structure

This directory follows the structure given in the lab assignment. It contains:
- A *src* directory, containing all the source code.
- `CMakeLists.txt` and `pico_sdk_import.cmake`. These are used for building source code.
- **IMPORTANT**: While it is not included, it is required to have the *pico-sdk* repository in the root (*embedded_driver*) directory.

## Building & Loading

This project uses the same build and load process as specified by the lab assignment.

1) Start from the *embedded_driver* root directory.
2) Clone *pico-sdk* to the root directory. 
3) `mkdir` and `cd` into a build directory.
4) From *build*, specify the path to the pico-sdk directory with the exact command:

<p align="center"><code>export PICO_SDK_PATH=../pico-sdk</code></p>

5) To build, run the following:

<p align="center"><code>cmake -DPICO_BOARD=adafruit_feather_rp2040 ..</code></p>

6) Run `make` to produce the required files. We are interested in `level.uf2` in particular.
7) Start the Feather RP2040 in BOOTSEL mode.
8) Drop `level.uf2` (or any executable) into the *RPI-RP2* drive. The Feather RP2040 will begin executing automatically.