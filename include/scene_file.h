#ifndef SCENE_FILE_H
#define SCENE_FILE_H

#include "HdrImage.h"
#include "camera.h"
#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "shapes.h"

using namespace std;

struct SourceLocation {

  string file_name = "";

  int line_num = 0;
  int col_num = 0;
  SourceLocation(){};
  SourceLocation(string, int, int);
};

#endif