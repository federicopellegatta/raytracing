#include "scene_file.h"
#include <cassert>
#include <sstream>

void test_input_file() {
  stringstream sstr("abc   \nd\nef");
  InputStream stream(sstr, SourceLocation());

  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 1);

  assert(stream.read_char() == 'a');
  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 2);

  stream.unread_char('A');
  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 1);

  assert(stream.read_char() == 'A');
  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 2);

  assert(stream.read_char() == 'b');
  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 3);

  assert(stream.read_char() == 'c');
  assert(stream.location.line_num == 1);
  assert(stream.location.col_num == 4);

  stream.skip_whitespaces_and_comments();

  assert(stream.read_char() == 'd');
  assert(stream.location.line_num == 2);
  assert(stream.location.col_num == 2);

  assert(stream.read_char() == '\n');
  assert(stream.location.line_num == 3);
  assert(stream.location.col_num == 1);

  assert(stream.read_char() == 'e');
  assert(stream.location.line_num == 3);
  assert(stream.location.col_num == 2);

  assert(stream.read_char() == 'f');
  assert(stream.location.line_num == 3);
  assert(stream.location.col_num == 3);

  assert(stream.read_char() == char_traits<char>::eof());
}
int main() {
  test_input_file();
  return 0;
}