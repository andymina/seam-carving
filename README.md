# Seam Carving

## Building from Source

To build from source, OpenCV must be installed from source with CMake. The path to the OpenCV build
folder must be updated in `CMakeLists.txt`. Python 3.6 or greater is required to build the
`seam_carving` module.

It is recommended to make a virtual environment as the following commands will install pip packages.
After building/installing dependencies run the following commands from the root directory:

1. `make setup`
2. `make build`

## Seam Carving Module

By default, the repo includes the latest standalone version of the `seam_carving` module at the root
named `seam_carving.so`. Python 3.10 or greater is required to run this specific build of the module. To
add support for older Python versions, you must build it from source yourself.

After building the module, import the standalone as `seam_carving` and begin using.

## Seam Carving Shell (SCS)

The Seam Carving Shell can be run from the `cli.py` file at the root: `python cli.py`. The cli
requires at least Python 3.10.

## TODO

- [ ] add Python documentation for C++ bindings in `bind.cpp`
- [ ] clean up branching code in `CarvableImage.cpp`
- [ ] add support for multisize images and real-time seam carving
- [ ] implement transport maps