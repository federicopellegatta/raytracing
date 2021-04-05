#include <HdrImage.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

  fmt::print("Hello world in Technicolor!\n");
  fmt::print("The first argument of argv[] is: {} \n", argv[0]);

  // Define default input parameters
  string input_pfm_file_name = "";
  float factor = 0.2;
  float gamma = 1.0;
  string output_png_file_name = "";

  // Read input from command-line
  if (argc != 5) {
    throw InvalidPfmFileFormat(
        "Usage: main.cpp INPUT_PFM_FILE FACTOR GAMMA OUTPUT_PNG_FILE");
    // return -1;
  }

  input_pfm_file_name = argv[1];
  output_png_file_name = argv[4];

  try {
    factor = atof(argv[2]);
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat("FACTOR is not a floating point");
  }

  try {
    gamma = atof(argv[3]);
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat("GAMMA is not a floating point");
  }

  // Open input PFM file
  ifstream in(input_pfm_file_name);
  HdrImage img(in);
  cout << "File " << input_pfm_file_name << " has been read from disk." << endl;
  in.close();

  // Convert PFM to LDR format
  img.normalize_image(factor);
  img.clamp_image();

  // Open output file
  ofstream out(output_png_file_name);
  // img.write_ldr_image(out, "PNG", gamma);
  cout << "File " << output_png_file_name << " has been written to disk."
       << endl;
  out.close();

  return 0;
}