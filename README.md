# miniRT
## Introduction
**Monte Carlo path tracer.**

> [!IMPORTANT]
> This is project is work in progress. It is still missing many of the planned features. Some of the parameters are currently hardcoded in and there may be bugs.

> [!NOTE]
> Some of the features, such as HDR images, utilisation of GPU, and a denoising solution, will not be implemented until the project has been validated. This is due to restrictions placed onto the subject.

> [!NOTE]
> [MLX42](https://github.com/codam-coding-college/MLX42), a minimal graphics library, is used in this project. It handles window creation and manages the framebuffer to which we copy ours.
>
> The `Makefile` will download and build MLX42, but it might require certain dependencies to be installed depending on your system. Please refer to their [installation documentation](https://github.com/codam-coding-college/MLX42?tab=readme-ov-file#for-linux) for more information.

<br>

## Features
- Monte Carlo integration with importance sampling for Global Illumination
- Bounding Volume Hierarchy for computing intersections
- Modest post-processing stack with ACES tonemapping

#### TODO
- Disney BRDF and approximated BSDF materials
- Additional primitive object
- Texture maps and procedural patterns
- Normal maps
- Extented camera properties and trackball controls
- Object editing with simplified rendering mode
- Additional post-processing modules
- Extented scene description format

<br>

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
| <kbd>Left ALT/CMD</kbd> + <kbd>Left Mouse Button</kbd> and drag                | Orbit Camera                                              |

### General

| ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Key⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                     | ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀Description⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀         |
|----------------------------------------------------------------|-----------------------------------------------------------|
| <kbd>Esc</kbd>                                                 | Quit                                                      |
