#!/bin/bash

if [ "$1" == "" ]; then
    echo "Usage: $(basename $0) NUM_OF_CORES"
    exit 1
fi

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cd "build/"
cmake --build .
if [ -d "demo_animation/" ]
then 
    rm -rf "demo_animation/"
	mkdir "demo_animation/"
fi
cd "demo_animation/"

parallel -j $1 ../../generate-image.sh '{}' ::: $(seq 0 359)

# -r 25: Number of frames per second
ffmpeg -r 25 -f image2 -s 640x360 -i image%03d.png \
    -vcodec libx264 -pix_fmt yuv420p \
    demo_animation.mp4

rm -r *.pfm
rm -r *.png
echo "The animation has been created in 'build/demo_animation'!"