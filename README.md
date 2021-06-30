
# raytracer
![Operating Systems](https://img.shields.io/badge/os-Linux-lightgrey)
[![CI](https://img.shields.io/github/workflow/status/federicopellegatta/raytracing/CMake)](https://github.com/federicopellegatta/raytracing/actions)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

raytracer is a C++ code that you can use to generate simple photorealistic images using spheres and planes. Secondly, it can be also used to convert a image from `pfm` to `png` or `jpeg` format. 


## Installation
Note that raytracer since v1.0.0 works only on Linux machines.
### Dependencies
   - C++ Compiler (at least C++17)
   - [CMake](https://cmake.org/) (at least version 3.16)
   - [GD library](https://libgd.github.io/) (at least version 2.5.5)
   - [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) (at least version 0.29)

If you want to generate animations, we also recommend:
   - [ffmpeg](https://www.ffmpeg.org/) (at least version 4.2.4)
   - [GNU parallel](https://www.gnu.org/software/parallel/) 

If you're willing to use [`conda`](https://docs.conda.io/en/latest/), or if you already do, all the above dependencies can be installed via
```sh
$ conda env create -n <envname> -f requirements.yml
```
Or you can use your distribution package manager to install them.
  
### Download and building
If all the dependencies have been met,
 - you can download the latest stable release [here](https://github.com/federicopellegatta/raytracing/releases), and unpack it
   ``` sh
   $ tar -xvf /path/to/tar #or zip file -C /path/to/your/directory
   ```
 - if you want to stay up to date with the latest changes (note that they might not work as intended), clone this repository
   ``` sh
   $ git clone https://github.com/federicopellegatta/raytracing.git
   ```

Next, follow the instructions below in order to compile raytracer in your own directory.
``` sh
$ cd /path/to/your/directory  
$ # (if you're using conda -> conda activate <envname>)
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 
$ cd build/
$ cmake --build . -j <NUM_OF_CORES> # We advise at least 2 cores, as the compilation process is a bit slow
```
You will find tests and executable files in `build` directory. Note that the executable main file is `raytracer`.

## Usage
To check if tests are passing do
``` sh
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug 
$ cd build
$ cmake --build . -j <NUM_OF_CORES>
$ ctest
```
If tests are not passing feel free to open an issue.

A help menu will be displayed by typing (the command-line interface is built using [Taywee/args](https://github.com/Taywee/args))
``` sh
$ ./raytracer --help
```
### Render
raytracer can generate images using 3 algorithms: 
 - *on-off renderer*: it produces black and white image. We recommed to use this algorithm in order to debug, because it is really fast;
 - *flat renderer*: it estimates the solution of the rendering equation by neglecting any contribution of the light. It just uses the pigment
 of each surface to determine how to compute the final radiance;
 - *pathtracing*: it allows the caller to tune number of rays thrown at each iteration, as well as the maximum depth. It implements Russian roulette, so in principle it will take a finite time to complete the calculation.
Inside the [`examples`](./examples) directory there are input files defining different scenes. 

>Note that the file [`examples/demo.txt`](./examples/demo.txt) contains instructions on how to write a correct input file.

The following command
``` sh
$ ./raytracer render -w 640 -h 360 --alg pathtracing --num-of-rays 5 --max-depth 4 --samples-per-pixel 25 --outf demo-5 -i ../examples/demo.txt
```
will generate the image below (which is defined in the input file [`examples/demo.txt`](./examples/demo.txt))

![Demo image](https://raw.githubusercontent.com/federicopellegatta/raytracing/master/examples/demo-5.png)


Thanks to `ffmpeg` and a couple of cli options it is possibile to generate simple animations; the scripts [`demo_animation.sh`](demo_animation.sh) and [`generate-image.sh`](generate-image.sh) facilitates this, and by launching
``` sh
$ ./demo_animation.sh -j <NUM_OF_CORES>
```
will produce

![Demo animation](https://github.com/federicopellegatta/raytracing/blob/master/examples/demo.gif)

Feel free to tweak the scripts to your own liking.


### Convert images from HDR to LDR format
raytracer can also convert images from HDR to LDR format (only `png` and `jpeg` are supported):  use the `convertpfm2png` command as in the following example:
```
$ ./raytracer convertpfm2png --inpfm=/path/to/pfm-file --outpng=/path/to/output-file -f 1.0 -g 1.0
```

## Contributing
If you wish to contribute or you have just found any bug, please open an issue or a pull request on our GitHub repository. Thank you!

## Licence
raytracer is licensed under the terms of the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) and is available for free. See the file [LICENCE.md](https://github.com/federicopellegatta/raytracing/blob/master/LICENCE.md).
