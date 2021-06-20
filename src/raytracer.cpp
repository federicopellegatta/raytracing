#include "HdrImage.h"
#include "args.hxx"
#include "camera.h"
#include "imagetracer.h"
#include "materials.h"
#include "render.h"
#include "scene_file.h"
#include "world.h"
#include <memory>

using namespace std;

void imagerender(int width, int height, string algorithm, int init_state,
                 int init_seq, int num_of_rays, int max_depth,
                 int samples_per_pixel, string output_file,
                 string input_scene) {

  // Checking if antialiasing feature is on, and propersly set
  int samples_per_side = static_cast<int>(sqrt(samples_per_pixel));
  if (pow(samples_per_side, 2) != samples_per_pixel) {
    fmt::print("ERROR: the number of samples per pixel ({}) must be a perfect "
               "square.\nExiting.\n",
               samples_per_pixel);
    exit(1);
  }

  // Checking if user defined an output file
  if (output_file == "")
    throw invalid_argument("You must specify the output filename");

  string format = static_cast<string>(output_file)
                      .erase(0, static_cast<string>(output_file).find(".") + 1);
  string name = static_cast<string>(output_file)
                    .substr(0, static_cast<string>(output_file).find("."));
  if (format != output_file) // if user have specified a format
    output_file = name;

  string pfm_output = output_file + ".pfm";
  string png_output;
  if (format == "jpeg" || format == "jpg" || format == "JPEG")
    png_output = output_file + ".jpg";
  else
    png_output = output_file + ".png";

  // Parsing the input file defining the scene
  ifstream scene_file(input_scene);
  if (scene_file.fail()) {
    fmt::print("ERROR: unable to open {} file\n", input_scene);
    exit(1);
  }
  InputStream stream(scene_file, input_scene);
  map<string, float> vars;
  Scene scene;
  try {
    scene = stream.parse_scene(vars);
  } catch (GrammarError &e) {
    fmt::print(e.what());
    exit(1);
  }

  // Allocating the image
  HdrImage image(width, height);

  // Allocating the tracer
  ImageTracer tracer(image, scene.camera, samples_per_side);

  // Allocating the user-chosen renderer
  shared_ptr<Renderer> renderer;
  if (algorithm == "onoff") {
    fmt::print("Using on/off renderer\n");
    renderer = make_shared<OnOffRenderer>(scene.world);
  } else if (algorithm == "flat") {
    fmt::print("Using flat renderer\n");
    renderer = make_shared<FlatRenderer>(scene.world);
  } else if (algorithm == "pathtracing") {
    fmt::print("Using a path tracer\n");
    renderer = make_shared<PathTracer>(
        scene.world, BLACK, PCG(init_state, init_seq), num_of_rays, max_depth);
  } else {
    fmt::print("Unknown renderer type.\nExiting.\n");
    exit(1);
  }

  // Rendering the image (time-consuming process, where the "magic" happens)
  tracer.fire_all_rays([&](const Ray &ray) { return (*renderer)(ray); });

  // Writing pfm file
  ofstream pfm_stream(pfm_output);
  tracer.image.write_pfm(pfm_stream, Endianness::little_endian);
  fmt::print("File {} has been written to disk\n", pfm_output);

  // Apply tone - mapping to the image
  tracer.image.normalize_image(1.0);
  tracer.image.clamp_image();

  // Writing image in ldr format (for now png)
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

  args::Command render(commands, "render",
                       "Use this command to produce an image");
  args::Command convertpfm2png(
      commands, "convertpfm2png",
      "Use this option to convert a HDR image to PNG format");

  args::Group arguments(parser, "help", args::Group::Validators::DontCare,
                        args::Options::Global);
  args::Group render_arguments(parser, "render_arguments",
                               args::Group::Validators::DontCare,
                               args::Options::Global);
  args::Group pfm2png_arguments(parser, "pfm2png arguments",
                                args::Group::Validators::DontCare,
                                args::Options::Global);
  args::HelpFlag help(arguments, "help", "Display this help menu", {"help"});
  args::ValueFlag<int> width(render_arguments, "",
                             "Width of the image to produce", {'w', "width"});
  args::ValueFlag<int> height(
      render_arguments, "", "Height of the image to produce", {'h', "height"});
  args::ValueFlag<string> algorithm(
      render_arguments, "",
      "Type of renderer to use to produce image, "
      "can either be 'flat', 'onoff' or 'pathtracing'",
      {"alg", "algorithm"});
  args::ValueFlag<string> output_filename(
      render_arguments, "",
      "Name of the output file. The program will produce two files: "
      "<outf>.pfm and <outf>.png (or <outf>.jpg)",
      {"outf", "output_filename"});
  args::ValueFlag<int> num_of_rays(
      render_arguments, "",
      "Number of rays departing from each surface point (only applicable with "
      "--algorithm=pathtracing).",
      {"num-of-rays"});
  args::ValueFlag<int> max_depth(render_arguments, "",
                                 "Maximum allowed ray depth (only applicable "
                                 "with --algorithm=pathtracing).",
                                 {"max-depth"});
  args::ValueFlag<int> init_state(
      render_arguments, "",
      "Initial seed for the random number generator (positive number).",
      {"init-state"});
  args::ValueFlag<int> init_seq(render_arguments, "",
                                "Identifier of the sequence produced by the "
                                "random number generator (positive number).",
                                {"init-seq"});
  args::ValueFlag<int> samples_per_pixel(
      render_arguments, "",
      "Number of samples per pixel (must be a perfect square, e.g., 16).",
      {"samples-per-pixel"});
  args::ValueFlag<string> scene_file(render_arguments, "",
                                     "Input file defining the scene.",
                                     {'i', "input-scene"});
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

  if (render) {
    imagerender(args::get(width), args::get(height), args::get(algorithm),
                args::get(init_state), args::get(init_seq),
                args::get(num_of_rays), args::get(max_depth),
                args::get(samples_per_pixel), args::get(output_filename),
                args::get(scene_file));
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