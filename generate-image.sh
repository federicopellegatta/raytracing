#!/bin/bash

if [ "$1" == "" ]; then
    echo "Usage: $(basename $0) ANGLE"
    exit 1
fi

readonly angle="$1"
readonly angleNNN=$(printf "%03d" $angle)
readonly pfmfile=image$angleNNN.pfm
readonly pngfile=image$angleNNN.png

time ../raytracer demo --width 640 --height 480 --deg $angle \
	--algorithm flat --outf $pfmfile \
	&& ../raytracer convertpfm2png --factor 1 --gamma 1 --luminosity 0.5 \
        --inpfm $pfmfile --outpng $pngfile