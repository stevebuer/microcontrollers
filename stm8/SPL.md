# SPL Notes

## Official STM8 SPL for commercial compilers

The offical ST SPL (STSW-STM8069) is designed for STM8S/A devices using the Cosmic or Raisonance compilers.

There are several ports of the library to SDCC available.

The library consists of a couple dozen header and C files.

## SDCC Ports

There are two major SDCC ports of the library:

⭐ STM8S-SDCC-SPL (Uli Köhler)

A minimal, SDCC‑friendly SPL designed to keep binaries small.

1. Functions are split into individual files to avoid code bloat.
2. Requires SDCC ≥ 3.4.3.
3. Ideal if you want tight control over code size.

⭐ STM8-SPL-SDCC (Bruno Schwander)

A more complete SPL port for SDCC, based on ST’s official STSW‑STM8069 package.

1. Includes patches for SDCC compatibility.
2. Splits functions into separate files for dead‑code elimination.
3. Comes with example projects and Makefiles.

## Experiments with both in ~/STM8
