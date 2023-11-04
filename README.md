# Seam Carving

A native C++ implementation of seam carving as described by Avidan et al. in [Seam carving for
content-aware image resizing](https://doi.org/10.1145/1275808.1276390). Learn more about seam carving from [this YouTube video](https://www.youtube.com/embed/6NcIJXTlugc).

![Seam Carving Demo GIF](docs/assets/seam-carving-demo.gif)

*Credit to [George Wang](https://inst.eecs.berkeley.edu/~cs194-26/sp20/upload/files/projFinalAssigned/cs194-26-aea/) for the GIF*

## Getting Started

The seam carving library can be built using CMake. The available targets are:

- `seam_carving` - builds the dylib
- `demo` - runs the demo at `./demo.cpp`
- `carver_unit/energy_unit` - runs the unit tests at `tests/unit/[carver/energy].test.cpp` respectively

### Running the demo

1. Configure the CMake project. The code sample below uses Ninja as the generator. There are presets in `CMakePresets.json`
available to use. 
2. Build the `demo` target.
3. Run the demo target with a path to an image (sample images are provided in `samples/`), and the target size

```
# step 1 - from the root folder of the repo
cmake -B {PATH_TO_BUILD_DIR} -S {PATH_TO_REPO_ROOT} -G Ninja --preset default

# step 2
cmake --build {PATH_TO_BUILD_DIR} --target demo

# step 3
./demo {PATH_TO_IMG} {TARGET COLS} {TARGET ROWS}
```

## Seam Carving Shell (SCS)

The Seam Carving Shell (SCS) is undergoing major reconstruction and is not supported in v2.0.0.
