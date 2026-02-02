# miniRT
## Introduction
**Monte Carlo path tracer.**

> [!IMPORTANT]
> This project is work in progress! It is still missing many of the planned features. Some of the parameters are currently hardcoded in and there may be bugs.
>
> Some of the features will not be implemented until the project has been validated. This is due to restrictions placed upon the subject.

> [!NOTE]
> [MLX42](https://github.com/codam-coding-college/MLX42), a minimal graphics library required by the subject, is used in this project. It handles window creation and manages a framebuffer to which we copy ours.
>
> The `Makefile` will download and build MLX42, but it might require certain dependencies to be installed on your system. Please refer to their [installation documentation](https://github.com/codam-coding-college/MLX42?tab=readme-ov-file#for-linux) for more information.

## Features
- Monte Carlo integration with importance sampling for Global Illumination
- Bounding Volume Hierarchy for computing intersections
- Modest post-processing stack with ACES tonemapping
- High-performance CPU parallelism with multi-threading, memory efficiency, and systems-level optimisations enabling vectorisation of data

#### TODO
- Disney BRDF and approximated BSDF materials
- Additional primitive objects
- Texture maps and procedural patterns
- Normal maps
- Extented camera properties and trackball controls
- Quaternions
- Object editing with simplified rendering mode
- Additional post-processing modules
- Extented scene description format

#### Post-validation Features
- Discard the restrictive coding standard required by the subject and refactor the code to be more efficient
- HDRi texture for skydome
- Denoising solution
- Load .ojb meshes
- Port the rendering kernel to CUDA and refactor data to Structure of Arrays (SoA) to ensure coalesced global memory access
- Replace MLX42 with GPU interop to render directly to the display buffer, avoiding data transfer back to CPU

## Optimisation & Performance
This project implements a highly optimised CPU rendering engine, balancing code readability with raw performance. By analysing assembly output with GDB and compiler behaviour, several optimisations were implemented to maximise throughput.

### Math Utilities
High-performance linear algebra library providing SIMD-accelerated, memory-aligned vector and matrix primitives.

### Blit Function


#### Vectorised Preview Mode
Used during the camera movement. Achieves SIMD (Single Instruction, Multiple Data) execution, processing 4 pixels in parallel per instruction cycle.

-

#### Pipelined Refine Mode

-

### Micro-optimisations

- Invariant caching to local variables to prevent pointer aliasing, ensuring the compiler uses registers instead of repeated L1 cache loads

- Branchless arithmetic to minimise CPU stalls in performance-critical loops.

## Physically Based Rendering

## Monte Carlo Integration



## How to use
Run the following commands to clone the repository and create `miniRT` program
``` git
git clone https://github.com/mordori/miniRT.git miniRT
cd miniRT
make
```

Run the program with a .rt scene file
``` bash
./miniRT assets/scenes/test.rt
```

To delete all of the generated files, use
``` Makefile
make fclean
```
<br>

## Scene description
The program takes a `.rt` file as an argument. Premade scenes can be found in `📁assets/scenes/`.

As the project is still under construction, we recommend to run the program with
``` bash
./miniRT assets/scenes/test.rt
```

<br>

## Controls

### Camera

| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                     | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀        |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Left ALT</kbd> + <kbd>Left Mouse Button</kbd> and drag                | Orbit Camera                                              |

### General

| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                     | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀         |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Esc</kbd>                                                 | Quit                                                      |
