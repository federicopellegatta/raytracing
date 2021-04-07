# Ray-Tracing Lab Project
Ray-Tracing is a C++ code that you can use to generate photorealistic images.

## Installation
### Dependencies
   - C++ Compiler
     - At least C++17
     - It can be installed via your distribution package manager or using `conda`
   - [`CMake`](https://cmake.org/)
     - At least version 3.16.3
     - It can be installed via your distribution package manager or using `conda`
   - [GD library](https://github.com/libgd/libgd)
     - At least version 2.5.5
     - It can be installed via your distribution package manager or using `conda`
### Download and building
If all the dependencies have been met, the following instructions should work.
```
$ git clone https://github.com/federicopellegatta/raytracing.git
$ cd raytracing && mkdir build && cd build
$ cmake ..
$ make
```
You will find tests and executable files in `build` directory. 

## Usage
Ray-Tracing can be used to convert images from HDR format to LDR. 
    - for instance the following command convert from `ofm` to `png`
    
      $ ./raytracer /path/to/pfmfile normalize_factor gamma /path/to/pngfile
      

## Contributing
If you wish to contribute or you have just found any bug, please open an issue or a pull request on our GitHub repository. Thank you!

## Licence
Ray-Tracing is licensed under the terms of the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) and is available for free. See the file [LICENCE.md](https://github.com/federicopellegatta/raytracing/blob/master/LICENCE.md)
