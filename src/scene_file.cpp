#include "scene_file.h"

SourceLocation::SourceLocation(string _file_name, int _line_num, int _col_num) {
  file_name = _file_name;
  line_num = _line_num;
  col_num = _col_num;
}
