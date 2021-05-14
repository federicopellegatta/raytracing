#include "HdrImage.h"
#include "args.hxx"
#include "camera.h"
#include "imagetracer.h"
#include "world.h"

using namespace std;

struct Demo {
  HdrImage image;
  World world;
  shared_ptr<Camera> camera;
  string pfm_output, png_output;

  Demo(int width, int height, float angle_deg, string camera_type,
       string output);

  void run();
};

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
  fmt::print("File {} has been written to disk\n", pfm_output);

  // Apply tone - mapping to the image
  tracer.image.normalize_image(1.0);
  tracer.image.clamp_image();

  tracer.image.write_ldr_image(png_output.c_str(), 1.0);
  fmt::print("File {} has been written to disk. \n", png_output);
}

struct pfm2png {
  HdrImage image;
  float factor = 1.0;
  float gamma = 1.0;
  string input_pfm_filename = "";
  string output_filename = "";

  pfm2png(string input_pfm_filename, string output_filename, float factor,
          float gamma);
};

pfm2png::pfm2png(string input_pfm_filename, string output_filename,
                 float _factor, float _gamma)
    : image(input_pfm_filename) {

  factor = _factor;
  gamma = _gamma;
  fmt::print("File {} has been read from disk. \n", input_pfm_filename);

  // Run Tone-Mapping
  image.normalize_image(factor);
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
  args::Group arguments(parser, "arguments", args::Group::Validators::DontCare,
                        args::Options::Global);
  args::HelpFlag help(arguments, "help", "Display this help menu",
                      {'h', "help"});
  try {
    parser.ParseCLI(argc, argv);
  } catch (const args::Help &) {
    std::cout << parser;
    return 0;
  } catch (const args::ParseError &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }
  if (demo) {
    Demo test(640, 480, 0, "perspective", "test");
    test.run();
  }
  if (convertpfm2png) {
    pfm2png("../test/HdrImage_references/memorial.pfm", "memorial.png", 0.5,
            2.1);
  }
  return 0;
  /*
  args::ArgumentParser p("git-like parser");
  args::Group commands(p, "commands");
  args::Command add(commands, "add", "add file contents to the index");
  args::Command commit(commands, "commit", "record changes to the repository");
  args::Group arguments(p, "arguments", args::Group::Validators::DontCare,
                        args::Options::Global);
  args::ValueFlag<std::string> gitdir(arguments, "path", "", {"git-dir"});
  args::HelpFlag h(arguments, "help", "help", {'h', "help"});
  args::PositionalList<std::string> pathsList(arguments, "paths",
                                              "files to commit");

  try {
    p.ParseCLI(argc, argv);
    if (add) {
      std::cout << "Add";
    } else {
      std::cout << "Commit";
    }

    for (auto &&path : pathsList) {
      std::cout << ' ' << path;
    }

    std::cout << std::endl;
  } catch (args::Help) {
    std::cout << p;
  } catch (args::Error &e) {
    std::cerr << e.what() << std::endl << p;
    return 1;
  }
  return 0;
  */
}

int main(int argc, char **argv) {
  interface(argc, argv);
  return 0;
}
