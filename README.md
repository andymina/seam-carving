# Seam Carving

A native C++ implementation of seam carving as described by Avidan et al. in [Seam carving for
content-aware image resizing](https://doi.org/10.1145/1275808.1276390). Learn more about seam carving from [this YouTube video](https://www.youtube.com/embed/6NcIJXTlugc).

![Seam Carving Demo GIF](docs/assets/seam-carving-demo.gif)

*Credit to [George Wang](https://inst.eecs.berkeley.edu/~cs194-26/sp20/upload/files/projFinalAssigned/cs194-26-aea/) for the GIF*

## Getting Started

It is recommended to make a virtual environment as the setup command will install pip packages.
This project uses [vcpkg](https://github.com/microsoft/vcpkg) as its C++ package manager and
[CMake](https://cmake.org) as the build-system.

Run `make setup` from the root directory to set up the project. This will create and populate a `build/` directory and create a new `out/` directory where output files will
go by default.

The following `make` commands are currently available:

- `make demo`: builds a C++17 executable to demo seam carving. Sample images are provided in the
`samples/` directory
- `make py`: exports the existing C++ seam carving code to a Python 3.10 package called `seam_carving`
- `make clean`: cleans the project and removes any build files

## Seam Carving Shell (SCS)

The Seam Carving Shell can be run from the `cli.py` file at the root: `python cli.py`. The cli
requires at least Python 3.10.
