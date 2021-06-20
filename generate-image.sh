#!/bin/bash

if [ "$1" == "" ]; then
    echo "Usage: $(basename $0) ANGLE"
    exit 1
fi

readonly angle_deg="$1"
readonly angleNNN=$(printf "%03d" $angle_deg)
readonly pfmfile=image$angleNNN.pfm
readonly pngfile=image$angleNNN.png

time ../raytracer render -w 640 -h 360 --alg pathtracing --num-of-rays 10 --max-depth 4 \
        --init-state 42 --init-seq 54 --samples-per-pixel 4 \
        --outf image$angleNNN -i ../../examples/demo.txt -d angle:$angle_deg \
        && ../raytracer convertpfm2png --factor 1 --gamma 1 --luminosity 0.5 \
        --inpfm $pfmfile --outpng $pngfile