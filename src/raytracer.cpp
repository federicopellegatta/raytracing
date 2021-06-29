/*
 * Copyright (c) 2021 Simone Pirota, Federico Pellegatta
 *
 * This file is part of raytracer.
 *
 * raytracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * raytracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with raytracer.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "HdrImage.h"
#include "args.hxx"
#include "camera.h"
#include "imagetracer.h"
#include "materials.h"
#include "render.h"
#include "scene_file.h"
#include "world.h"
#include <chrono>
#include <memory>

using namespace std;

/**
 * @brief Timer class to easily time code execution
 * Highly inspired (if not entirely copied) from
 * https://www.learncpp.com/cpp-tutorial/timing-your-code/
 *
 */
struct Timer {
private:
  // Type aliases to make accessing nested type easier
  using clock_t = chrono::high_resolution_clock;
  using second_t = chrono::duration<double, ratio<1>>;

  chrono::time_point<clock_t> m_beg;

public:
  /**
   * @brief Construct a new Timer object with the time of calling
   *
   */
  Timer() : m_beg{clock_t::now()} {}
  /**
   * @brief Resets the timer
   *
   */
  void reset() { m_beg = clock_t::now(); }

  /**
   * @brief Returns the elapsed time in proper units
   *
   * @return double
   */
  double elapsed() const {
    return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
  }
};

/**
 * @brief Split a string (using boost would be better, but for only one
 * function it would have been overkill). Method found here:
 * https://stackoverflow.com/questions/289347/using-strtok-with-a-stdstring
 *
 * @param str
 * @param delim
 * @param parts
 */
void split(const string &str, const string &delim, vector<string> &parts) {
  size_t start, end = 0;
  while (end < str.size()) {
    start = end;
    while (start < str.size() && (delim.find(str[start]) != string::npos)) {
      start++; // skip initial whitespace
    }
    end = start;
    while (end < str.size() && (delim.find(str[end]) == string::npos)) {
      end++; // skip to end of word
    }
    if (end - start != 0) { // just ignore zero-length strings.
      parts.push_back(string(str, start, end - start));
    }
  }
}

/**
 * @brief Parse the list of `-d` switches and return a map associating
 * variable names with their values
 *
 * @param definitions
 * @return map<string,float>
 */
map<string, float> build_vars_table(vector<string> &definitions) {
  map<string, float> vars;
  for (const auto &el : definitions) {
    vector<string> parts;
    split(el, ":", parts);
    if (parts.size() != 2) {
      fmt::print(
          "ERROR: the definition {} does not follow the pattern NAME:VALUE\n",
          el);
      exit(1);
    }
    string name = parts.at(0), value = parts.at(1);
    float f_value = 0.;
    try {
      f_value = stof(value);
    } catch (invalid_argument &e) {
      fmt::print("invalid floating point value {} in definition {}\n", value,
                 el);
    }
    vars[name] = f_value;
  }

  return vars;
}

void imagerender(int width, int height, string algorithm, int init_state,
                 int init_seq, int num_of_rays, int max_depth,
                 int samples_per_pixel, string output_file, string input_scene,
                 vector<string> &cli_vars) {

  // Checking if antialiasing feature is on, and properly set
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
  map<string, float> vars = build_vars_table(cli_vars);
  Scene scene;
  try {
    scene = stream.parse_scene(vars);
  } catch (GrammarError &e) {
    fmt::print(e.what());
    exit(1);
  }

  /* Warn the user if the aspect_ratio specified by CLI or in input file is
   * different from width/height */
  float _expected_aspect_ratio = static_cast<float>(width) / height;
  if (!are_close(scene.camera->aspect_ratio, _expected_aspect_ratio, 1e-3)) {
    fmt::print(
        "The aspect ratio you defined ({}) is not the ideal one ({}) for this "
        "dimensions\nNote that the aspect ratio should be width:height\n",
        scene.camera->aspect_ratio, _expected_aspect_ratio);
    string answer;
    fmt::print("Do you want to change it? [Y|N] ");
    cin >> answer;
    if (answer == "Y" || answer == "yes" || answer == "Yes" || answer == "y") {
      float new_aspect_ratio;
      fmt::print("Insert new value for aspect ratio: ");
      cin >> new_aspect_ratio;
      scene.camera->aspect_ratio = new_aspect_ratio;
    } else if (answer == "N" || answer == "no" || answer == "No" ||
               answer == "n") {
      fmt::print("Okay, your image your choice, don't blame me if it'll come "
                 "out strange\n");
    } else {
      fmt::print("Answer not recognized. Exiting\n");
      exit(1);
    }
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

  Timer t;
  // Rendering the image (time-consuming process, where the "magic" happens)
  tracer.fire_all_rays([&](const Ray &ray) { return (*renderer)(ray); });
  fmt::print("Rendering completed in {} s\n", t.elapsed());

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
  args::ValueFlagList<string> declare_float(
      render_arguments, "",
      "Declare a variable. The syntax is «--declare-float=VAR:VALUE». Example: "
      "--declare-float=clock:150",
      {'d', "declare-float"});
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
    vector<string> cli_vars;
    if (declare_float) {
      for (const auto &str : args::get(declare_float)) {
        cli_vars.push_back(str);
      }
    }
    imagerender(args::get(width), args::get(height), args::get(algorithm),
                args::get(init_state), args::get(init_seq),
                args::get(num_of_rays), args::get(max_depth),
                args::get(samples_per_pixel), args::get(output_filename),
                args::get(scene_file), cli_vars);
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