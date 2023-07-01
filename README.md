# HOG-feature-descriptor
The HOG Feature Descriptor Library is a C++ library that allows you to work with Histogram of Oriented Gradient (HOG) features on images. It provides functionalities for vector visualization, feature extraction using the `HOGDescriptor` class, and a Tex-based visualization process using the `texHOG` class.

## Table of Contents
- [Introduction](#introduction)
- [Installation](#installation)
  - [Windows](#windows)
  - [macOS/Linux](#macoslinux)
- [Building with Console Application and Doxygen Documentation](#building-with-console-application-and-doxygen-documentation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction
The Histogram of Oriented Gradient (HOG) is a widely used feature descriptor for object detection and recognition in computer vision. This library provides an easy-to-use interface to work with HOG features on images. With its integration with OpenCV, it offers efficient and reliable feature extraction and visualization.

## Installation
Before installing the HOG Feature Descriptor Library, make sure you have the following dependencies:
- CMake
- OpenCV (installed with vcpkg is preferred)

### Windows
To install the library on Windows, follow these steps:

1. Open a command prompt or terminal in the root directory of the project.
2. Run the following commands:
```
   cmake -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg_toolchain] -DCMAKE_INSTALL_PREFIX=[installation_path] -B ./build
   cd ./build
   cmake --build . --target install
```
### macOS/Linux
1. Open a terminal and navigate to the project directory.
2. Run the following commands:
```
cmake -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg_toolchain] -DCMAKE_INSTALL_PREFIX=[installation_path] -B ./build
cd ./build
cmake --build .
cmake --install .
```

Please replace `[path_to_vcpkg_toolchain]` with the actual path to the vcpkg toolchain file, and `[installation_path]` with the desired installation path.

## Building with Console Application and Doxygen Documentation
To include the console application and generate Doxygen documentation, use the following CMake flags during the build:

`-DSAMPLE=ON`: Include the console application in the build.

`-DDOXYGEN=ON`: Generate Doxygen documentation.

## Usage
Some examples of how to use the HOG Feature Descriptor Library in your C++ project can be found in `src/example` folder

## Contributing
Contributions to the HOG Feature Descriptor Library are welcome! If you find any bugs or have suggestions for improvement, please submit an issue or a pull request on the GitHub repository.

## License
The HOG Feature Descriptor Library is released under the MIT License.

Done as a course work for "OOP" 2023 class in NUST MISIS
