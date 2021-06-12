#include "scene_file.h"
#include <cassert>
#include <sstream>

template <typename Base, typename T> inline bool isinstanceof(const T) {
  return is_base_of<Base, T>::value;
}

void test_input_file() {
  stringstream sstr("abc   \nd\nef");
  InputStream stream(sstr);

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

  assert(stream.read_char() == '\0');
}

void test_lexer() {
  stringstream sstr(
      "\n# This is a comment\n# This is another comment\nnew material "
      "sky_material(\ndiffuse(image(\"my file.pfm\")),\n<5.0, 500.0, 300.0>\n) "
      "# Comment at the end of the line\n");

  InputStream stream(sstr);

  assert(stream.read_token().value.keyword == KeywordEnum::NEW);
  assert(stream.read_token().value.keyword == KeywordEnum::MATERIAL);
  assert(stream.read_token().value.str == "sky_material");
  assert(stream.read_token().value.symbol == '(');
  assert(stream.read_token().value.keyword == KeywordEnum::DIFFUSE);
  assert(stream.read_token().value.symbol == '(');
  assert(stream.read_token().value.keyword == KeywordEnum::IMAGE);
  assert(stream.read_token().value.symbol == '(');
  assert(stream.read_token().value.str == "my file.pfm");
  assert(stream.read_token().value.symbol == ')');
  assert(stream.read_token().value.symbol == ')');
  assert(stream.read_token().value.symbol == ',');
  assert(stream.read_token().value.symbol == '<');
  assert(stream.read_token().value.number == 5.0);
  assert(stream.read_token().value.symbol == ',');
  assert(stream.read_token().value.number == 500.0);
  assert(stream.read_token().value.symbol == ',');
  assert(stream.read_token().value.number == 300.0);
  assert(stream.read_token().value.symbol == '>');
  assert(stream.read_token().value.symbol == ')');
  stream.skip_whitespaces_and_comments();
  assert(stream.read_char() == '\0');
  // assert(isinstanceof<StopToken>(stream.read_token()));
}

int main() {
  test_input_file();
  test_lexer();
  return 0;
}