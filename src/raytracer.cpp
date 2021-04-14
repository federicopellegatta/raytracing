#include "HdrImage.h"

using namespace std;

struct Parameters {
  string input_pfm_filename = "";
  float factor = 0.2;
  float gamma = 1.0;
  string output_png_filename = "";

  void parse_line_arguments(int argc, char **argv);
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