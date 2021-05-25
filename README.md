
# Ray-Tracing
![Operating Systems](https://img.shields.io/badge/OS-Linux%20%7C%20MacOS-lightgrey)
[![CI](https://img.shields.io/github/workflow/status/federicopellegatta/raytracing/CMake)](https://github.com/federicopellegatta/raytracing/actions)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

Ray-Tracing is a C++ code that you can use to generate photorealistic images.

## Installation
### Dependencies
   - C++ Compiler (at least C++17)
   - [CMake](https://cmake.org/) (at least version 3.16)
   - [GD library](https://libgd.github.io/) (at least version 2.5.5)
   - [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) (at least version 0.29)

If you want to generate animation, we recommend also:
   - [ffmpeg](https://www.ffmpeg.org/) (at least version 4.2.4)
   - [GNU parallel](https://www.gnu.org/software/parallel/) 

All these dependencies can be installed via your distribution package manager or using [`conda`](https://docs.conda.io/en/latest/). 
  
### Download and building
If all the dependencies have been met, follow the instructions below in order to clone and compile Ray-Tracing in your own directory.
```sh
$ git clone https://github.com/federicopellegatta/raytracing.git
$ cd raytracing && mkdir build && cd build
$ cmake ..
$ make
```
You will find tests and executable files in `build` directory. 

## Usage
You can display a help menu typing (in `build` directory)
 ```sh
$ ./imagetracer --help
```

### Demo
Ray-Tracing can generate images. You can see a preview of Ray-Tracing ability using `demo` command; e.g. 
```sh
./raytracer demo -w 480 -h 480 --outf="demo.png" --alg="onoff"
```
All the options available can be displayed using `--help` command.

Enjoy the following demo animation running `./demo_animation.sh`

![Demo animation](https://i.imgur.com/thXAn05.gif)

### Convert images from HDR to LDR format
Ray-Tracing can also convert images from HDR to LDR format (only `png` and `jpeg` are supported):  use the `convertpfm2png` command as in the following example:
```sh
./raytracer convertpfm2png --inpfm="memorial.pfm" --outpng="memorial.png" -f 1.0 -g 1.0
```

## Contributing
If you wish to contribute or you have just found any bug, please open an issue or a pull request on our GitHub repository. Thank you!

## Licence
Ray-Tracing is licensed under the terms of the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) and is available for free. See the file [LICENCE.md](https://github.com/federicopellegatta/raytracing/blob/master/LICENCE.md).
