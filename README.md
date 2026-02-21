# miniRT
## Introduction
**High-Performance CPU Path Tracer with Monte Carlo Integration.**

<p align="center">
	<img src="doc/ball.gif" alt="Demo" />
</p>

> [!IMPORTANT]
>
> ⚠ This project is work in progress! ⚠
>
> Some of the features will not be implemented until the project has been validated. This is due to restrictions placed upon the subject.

> [!NOTE]
>
> Includes [MLX42](https://github.com/codam-coding-college/MLX42), a minimal graphics library required by the subject. It handles window creation and manages a frame buffer to which we copy ours.
>
> The `Makefile` will download and build MLX42, but it might require certain dependencies to be installed on your system. Refer to their [installation documentation](https://github.com/codam-coding-college/MLX42?tab=readme-ov-file#for-linux) for more information.
> Please ignore the section about XServer as it is no longer required for WSL2.

<p align="center">
	<img src="doc/rt.png" alt="Demo" />
</p>

## Features
- Monte Carlo integration with importance sampling for Global Illumination
- BVH acceleration structure for rapid intersection testing
- Modest post-processing stack with ACES-calibrated tonemapping
- High-performance CPU parallelism with multi-threading, memory efficiency, and systems-level optimisations enabling vectorisation of data
- Modern PBR pipeline with microfacet BSDF

#### TODO
- Anisotropic & Clear coat BRDF
- BTDF for BSDF
- Procedural patterns
- Normal maps
- Extented camera properties and trackball controls
- Quaternions
- Object editing

#### Future Work
- Discard the restrictive coding standard required by the subject and refactor to more efficient code
- Image based lighting
- Additional post-processing modules
- Denoising solution
- Quads and boxes
- Loading .ojb meshes
- Improve the BVH
- Port the rendering kernel to CUDA and refactor data to Structure of Arrays (SoA) to ensure coalesced global memory access
- Replace MLX42 with GPU interop to render directly to the display buffer, avoiding data transfer back to CPU

## Physically Based Rendering
Principled BSDF featuring Disney diffuse and Cook-Torrance specular.

- Implements Dupuy's 2023 spherical cap VNDF sampling method for efficient visible indirect normal generation
- Height-correlated Smith G2 visibility and analytical Smith G1 to quarantee MIS weight cancellation
- Singularity elimination in VNDF PDF to maintain 32-bit floating point stability
- Firefly mitigation and defence against variance spikes with indirect weight clamping, path roughing, and NaN/Inf sanitisation

## Optimisation & Performance
Implements a highly optimised CPU rendering engine, balancing code readability with raw performance.

By analysing assembly output with GDB and compiler behaviour, several optimisations were implemented to maximise throughput with auto-vectorisation. Building on these performance gains, data structures were vectorised explicitly.

Our approach optimises memory alignment for SIMD (Single Instruction, Multiple Data) execution while delegating most of the instruction selection to the compiler, maintaining a clean codebase.

### Math Utilities
- High-performance linear algebra library providing SIMD-accelerated, memory-aligned vector and matrix primitives.

### Blit Function
#### Vectorised Preview Mode
- Used during the camera movement. Achieves pure SIMD execution, processing 4 pixels in parallel per instruction cycle to maximize frame rate at the cost of slight image quality reduction.

#### Pipelined Rendered Mode
- Leverages loop unrolling to maximize Instruction Level Parallelism (ILP). While color channel dependencies limit vectorisation in this stage, unrolling reduces branch prediction overhead and saturates the CPU's superscalar execution.

### BVH
- Documentation under construction

### Micro-optimisations
- Invariant caching to local variables to prevent pointer aliasing.
- Branchless arithmetic to minimise CPU stalls in performance-critical loops.
- Strategically segregated write-heavy synchronization primitives from read-only render data to eliminate false sharing and cache line invalidation.

## Monte Carlo Integration

- Documentation under construction

## How to use
> [!NOTE]
> The default build configuration targets the Haswell microarchitecture (Intel 2013+ / AMD Ryzen).
>
> If you are compiling on legacy hardware, please replace `-march=haswell` with `-msse4.1` in the Makefile to ensure compatibility.

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

## Scene description
Premade scenes can be found in `📁assets/scenes/`.

As the project is still under construction, we recommend to run the program with
``` bash
./miniRT assets/scenes/test.rt
```

## Controls
### Rendering mode
| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                    | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀        |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Left Mouse Button</kbd> + drag                            | Rotate Camera                                             |
| <kbd>WASD</kbd>                                                | Move Camera                                               |
| <kbd>Space</kbd> / <kbd>Left Shift</kbd>                       | Elevate / Lower Camera                                    |

### Editing mode
| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                    | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀        |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Tab</kbd>                                                 | Edit mode                                                 |

### General
| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                    | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀        |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Left / Right Arrow</kbd>                                  | Rotate skydome                                            |
| <kbd>Esc</kbd>                                                 | Quit                                                      |

## Resources
Ray/path tracing
- https://raytracing.github.io/
- https://scratchapixel.com/index.html
- https://gabrielgambetta.com/computer-graphics-from-scratch/
- https://blog.selfshadow.com/
- https://users.aalto.fi/~lehtinj7/CS-E5520/2023/
- https://jacco.ompf2.com/articles/
- The Ray Tracer Challenge by Jamis Buck

PBR
- https://www.pbr-book.org/
- https://google.github.io/filament/main/filament.html#materialsystem/specularbrdf
- https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf

VNDF
- https://arxiv.org/pdf/2306.05044
- https://www.youtube.com/watch?v=u7TafvTVmbo

Blue noise
- https://momentsingraphics.de/BlueNoise.html

Math
- https://immersivemath.com/ila/index.html
- https://www.3blue1brown.com/
- https://betterexplained.com/
- https://gamemath.com/book/intro.html
