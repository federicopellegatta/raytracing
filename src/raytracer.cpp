#include "HdrImage.h"
#include <iostream>

using namespace std;

struct Parameters {
  string input_pfm_filename = "";
  float factor = 0.2;
  float gamma = 1.0;
  string output_png_filename = "";

  void parse_line_arguments(int argc, char **argv) {
    if (argc != 5) {
      throw runtime_error(
          "Usage: ./raytracer INPUT_PFM_FILE FACTOR GAMMA OUTPUT_PNG_FILE \n");
    }

    input_pfm_filename = argv[1];

    try {
      factor = atof(argv[2]);
    } catch (invalid_argument) {
      throw invalid_argument("FACTOR is not a floating point number");
    }

    try {
      gamma = atof(argv[3]);
    } catch (invalid_argument) {
      throw invalid_argument("GAMMA is not a floating point number");
    }

    output_png_filename = argv[4];
  }
};

int main(int argc, char **argv) {

  Parameters parameters;

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
             parameters.output_png_filename);

  return 0;
}