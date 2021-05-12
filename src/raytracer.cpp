#include "HdrImage.h"
#include "camera.h"
#include "imagetracer.h"
#include "world.h"

using namespace std;

struct Parameters {
  string input_pfm_filename = "";
  float factor = 0.2;
  float gamma = 1.0;
  string output_png_filename = "";

  void parse_line_arguments(int argc, char **argv);
};

struct Demo {
  HdrImage image;
  World world;
  shared_ptr<Camera> camera;
  string pfm_output, png_output;

  Demo(int width, int height, float angle_deg, string camera_type,
       string output);

  void run();
  void save_pfm();
  void save_png();
};

int main(int argc, char **argv) {

  /*Parameters parameters;

  // Read input from command-line
  parameters.parse_line_arguments(argc, argv);

  // Open input PFM file
  HdrImage img(parameters.input_pfm_filename);
  fmt::print("File {} has been read from disk. \n",
             parameters.input_pfm_filename);

  // Run Tone-Mapping
  img.normalize_image(parameters.factor);
  img.clamp_image();

  // Open output file
  img.write_ldr_image(parameters.output_png_filename.c_str(), parameters.gamma);
  fmt::print("File {} has been written to disk. \n",
             parameters.output_png_filename);*/

  Demo demo(640, 480, 0, "perspective", "demo");
  demo.run();

  return 0;
}

void Parameters::parse_line_arguments(int argc, char **argv) {
  if (argc != 5) {
    throw runtime_error("Usage: ./raytracer INPUT_PFM_FILE FACTOR GAMMA "
                        "OUTPUT_PNG-OR-JPEG_FILE \n");
  }

  input_pfm_filename = argv[1];
  factor = atof(argv[2]);
  gamma = atof(argv[3]);
  output_png_filename = argv[4];
}

Demo::Demo(int width, int height, float angle_deg, string camera_type,
           string output)
    : image(width, height) {
  // Create a world and populate it with a few shapes
  for (int i{}; i < 2; i++) {
    for (int j{}; j < 2; j++) {
      for (int k{}; k < 2; k++) {
        float x = 0.5 - i;
        float y = 0.5 - j;
        float z = 0.5 - k;
        world.add(make_shared<Sphere>(
            Sphere{translation(Vec(x, y, z)) * scaling(Vec(0.1, 0.1, 0.1))}));
      }
    }
  }

  // Place two other balls in the bottom / left part of the cube,so that we can
  // check if there are issues with the orientation of the image
  world.add(make_shared<Sphere>(
      Sphere{translation(Vec(0.0, 0.0, -0.5)) * scaling(Vec(0.1, 0.1, 0.1))}));
  world.add(make_shared<Sphere>(
      Sphere{translation(Vec(0.0, 0.5, 0.0)) * scaling(Vec(0.1, 0.1, 0.1))}));

  // Initialize camera
  float angle_rad = angle_deg * M_PI / 180;
  Transformation camera_tr =
      rotation_z(angle_rad) * translation(Vec(-1.0, 0.0, 0.0));

  if (camera_type == "orthogonal" || camera_type == "orthogonalCamera")
    camera = make_shared<OrthogonalCamera>(OrthogonalCamera(
        static_cast<float>(width) / static_cast<float>(height), camera_tr));
  else
    camera = make_shared<PerspectiveCamera>(PerspectiveCamera(
        1., static_cast<float>(width) / static_cast<float>(height), camera_tr));

  // if() check not format passing
  pfm_output = output + ".pfm";
  png_output = output + ".png";
}

void Demo::run() {
  ImageTracer tracer(image, camera);
  tracer.fire_all_rays([&](Ray ray) -> Color {
    if (world.ray_intersection(ray).hit) {
      return Color{1.0, 1.0, 1.0};
    } else {
      return Color{0.0, 0.0, 0.0};
    }
  });

  ofstream stream(pfm_output);
  tracer.image.write_pfm(stream, Endianness::little_endian);
  fmt::print("HDR demo image written to {}\n", pfm_output);

  // Apply tone - mapping to the image
  tracer.image.normalize_image(1.0);
  tracer.image.clamp_image();

  tracer.image.write_ldr_image(png_output.c_str(), 1.0);
  fmt::print("File {} has been written to disk. \n", png_output);
}
