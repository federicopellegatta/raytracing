cd "build/"
make
if [ ! -d "demo_animation/" ]
then 
	mkdir "demo_animation/"
fi
cd "demo_animation/"

for angle in $(seq 0 359); do
    # Angle with three digits, e.g. angle="1" → angleNNN="001"
    angleNNN=$(printf "%03d" $angle)
    ../raytracer demo --width=480 --height=480 --deg=$angle --outf=img$angleNNN
done

# -r 25: Number of frames per second
ffmpeg -r 25 -f image2 -s 480x480 -i img%03d.png \
    -vcodec libx264 -pix_fmt yuv420p \
    demo_animation.mp4

rm -r *.pfm
rm -r *.png
echo "The animation has been created in 'build/demo_animation'!"