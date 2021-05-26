#include "HdrImage.h"
#include "args.hxx"
#include "camera.h"
#include "imagetracer.h"
#include "materials.h"
#include "render.h"
#include "world.h"
#include <memory>

using namespace std;

struct Demo {
  HdrImage image;
  World world;
  shared_ptr<Camera> camera;
  string pfm_output, png_output;

  Demo(int width, int height, float angle_deg, string camera_type,
       string output);

  void run(string);
};

Demo::Demo(int width, int height, float angle_deg, string camera_type,
           string output)
    : image(width, height) {
  // Defining materials
  Material material1(make_shared<DiffusiveBRDF>(
      make_shared<UniformPigment>(Color(0.7, 0.3, 0.2))));
  Material material2(make_shared<DiffusiveBRDF>(make_shared<CheckeredPigment>(
      Color(0.2, 0.7, 0.3), Color(0.3, 0.2, 0.7), 4)));
  HdrImage sphere_texture(2, 2);
  sphere_texture.set_pixel(0, 0, Color(0.1, 0.2, 0.3));
  sphere_texture.set_pixel(0, 1, Color(0.2, 0.1, 0.3));
  sphere_texture.set_pixel(1, 0, Color(0.3, 0.2, 0.1));
  sphere_texture.set_pixel(1, 1, Color(0.3, 0.1, 0.2));

  Material material3(
      make_shared<DiffusiveBRDF>(make_shared<ImagePigment>(sphere_texture)));
  // Create a world and populate it with a few shapes
  for (int i{}; i < 2; i++) {
    for (int j{}; j < 2; j++) {
      for (int k{}; k < 2; k++) {
        float x = 0.5 - i;
        float y = 0.5 - j;
        float z = 0.5 - k;
        world.add(make_shared<Sphere>(
            Sphere{translation(Vec(x, y, z)) * scaling(Vec(0.1, 0.1, 0.1)),
                   material1}));
      }
    }
  }

  // Place two other balls in the bottom / left part of the cube,so that we can
  // check if there are issues with the orientation of the image
  world.add(make_shared<Sphere>(
      Sphere{translation(Vec(0.0, 0.0, -0.5)) * scaling(Vec(0.1, 0.1, 0.1)),
             material2}));
  world.add(make_shared<Sphere>(
      Sphere{translation(Vec(0.0, 0.5, 0.0)) * scaling(Vec(0.1, 0.1, 0.1)),
             material3}));

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

  if (output == "") // if user have not specify the output
    throw invalid_argument("You must specify the output filename");

  string format = static_cast<string>(output).erase(
      0, static_cast<string>(output).find(".") + 1);
  string name = static_cast<string>(output).substr(
      0, static_cast<string>(output).find("."));
  if (format != output) // if user have specified a format
    output = name;

  pfm_output = output + ".pfm";
  if (format == "jpeg" || format == "jpg" || format == "JPEG")
    png_output = output + ".jpg";
  else
    png_output = output + ".png";
}

void Demo::run(string algorithm) {
  shared_ptr<Renderer> renderer;

  if (algorithm == "onoff") {
    fmt::print("Using on/off renderer\n");
    renderer = make_shared<OnOffRenderer>(world);
  } else if (algorithm == "flat") {
    fmt::print("Using flat renderer\n");
    renderer = make_shared<FlatRenderer>(world);
  } else {
    fmt::print("Unknown renderer type.\nExiting.\n");
    exit(1);
  }

  ImageTracer tracer(image, camera);
  tracer.fire_all_rays([&](const Ray &ray) { return (*renderer)(ray); });

  ofstream stream(pfm_output);
  tracer.image.write_pfm(stream, Endianness::little_endian);
  fmt::print("File {} has been written to disk\n", pfm_output);

  // Apply tone - mapping to the image
  tracer.image.normalize_image(1.0);
  tracer.image.clamp_image();

  tracer.image.write_ldr_image(png_output.c_str(), 1.0);
  fmt::print("File {} has been written to disk. \n", png_output);
}

struct pfm2png {
  HdrImage image;
  float factor;
  float gamma;
  float luminosity;
  string input_pfm_filename;
  string output_filename;

  pfm2png(string, string, float, float, float);
};

pfm2png::pfm2png(string input_pfm_filename, string output_filename,
                 float _factor = 1., float _gamma = 1., float _luminosity = 0.)
    : image(input_pfm_filename) {

  factor = _factor;
  gamma = _gamma;
  luminosity = _luminosity;
  fmt::print("File {} has been read from disk. \n", input_pfm_filename);

  // Run Tone-Mapping
  if (luminosity == 0.) {
    image.normalize_image(factor);
  } else {
    image.normalize_image(factor, luminosity);
  }
  image.clamp_image();

  // Open output file
  image.write_ldr_image(output_filename.c_str(), gamma);
  fmt::print("File {} has been written to disk. \n", output_filename);
}

int interface(int argc, char **argv) {
  args::ArgumentParser parser(
      "Raytracing is a program that can generate photorealistic images.");
  args::Group commands(parser, "commands");

  args::Command demo(commands, "demo", "Use this command to produce an image");
  args::Command convertpfm2png(
      commands, "convertpfm2png",
      "Use this option to convert a HDR image to PNG format");

  args::Group arguments(parser, "help", args::Group::Validators::DontCare,
                        args::Options::Global);
  args::Group demo_arguments(parser, "demo arguments",
                             args::Group::Validators::DontCare,
                             args::Options::Global);
  args::Group pfm2png_arguments(parser, "pfm2png arguments",
                                args::Group::Validators::DontCare,
                                args::Options::Global);
  args::HelpFlag help(arguments, "help", "Display this help menu", {"help"});
  args::ValueFlag<int> width(demo_arguments, "",
                             "Width of the image to produce", {'w', "width"});
  args::ValueFlag<int> height(
      demo_arguments, "", "Height of the image to produce", {'h', "height"});
  args::ValueFlag<float> angle_deg(
      demo_arguments, "", "Angle in degrees of the pov of the image to produce",
      {"deg", "degrees"});
  args::ValueFlag<string> camera(
      demo_arguments, "",
      "Type of camera to use, can be either 'perspective' or 'orthogonal'",
      {"cam", "camera"});
  args::ValueFlag<string> algorithm(demo_arguments, "",
                                    "Type of renderer to use to produce image, "
                                    "can either be 'flat' or 'onoff'",
                                    {"alg", "algorithm"});
  args::ValueFlag<string> output_filename(
      demo_arguments, "",
      "Name of the output file. The program will produce two files: "
      "<outf>.pfm and <outf>.png (or <outf>.jpg)",
      {"outf", "output_filename"});
  args::ValueFlag<string> input_pfm(
      pfm2png_arguments, "", "Path to input pfm file", {"inpfm", "input_pfm"});
  args::ValueFlag<string> output_png(pfm2png_arguments, "",
                                     "Path to output png file",
                                     {"outpng", "output_png"});
  args::ValueFlag<float> factor(pfm2png_arguments, "", "Normalization factor",
                                {'f', "factor"});
  args::ValueFlag<float> luminosity(
      pfm2png_arguments, "",
      "Luminosity. Use 0 and it will be calculated "
      "internally.",
      {'l', "luminosity"});
  args::ValueFlag<float> gamma(pfm2png_arguments, "",
                               "Gamma factor of the screen to use",
                               {'g', "gamma"});

  try {
    parser.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  } catch (args::ValidationError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  if (demo) {
    Demo test(args::get(width), args::get(height), args::get(angle_deg),
              args::get(camera), args::get(output_filename));
    test.run(args::get(algorithm));
  }
  if (convertpfm2png) {
    pfm2png(args::get(input_pfm), args::get(output_png), args::get(factor),
            args::get(gamma), args::get(luminosity));
  }
  return 0;
}

int main(int argc, char **argv) {
  interface(argc, argv);
  return 0;
}
