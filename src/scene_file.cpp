#include "scene_file.h"

SourceLocation::SourceLocation(string _file_name, int _line_num, int _col_num) {
  file_name = _file_name;
  line_num = _line_num;
  col_num = _col_num;
}

void InputStream::update_pos(char ch) {
  if (ch == '\0') {
    // Notihing to do
    return;
  } else if (ch == '\n') {
    location.line_num += 1;
    location.col_num = 1;
  } else if (ch == '\t')
    location.col_num += tabulations;
  else
    location.col_num += 1;
}

char InputStream::read_char() {
  char ch;
  // Read a new character from the stream
  if (saved_char != '\0') {
    // Recover the «unread» character and return it
    ch = saved_char;
    saved_char = '\0';
  } else {
    // Read a new character from the stream
    stream.get(ch);
  }

  saved_location = location;
  update_pos(ch);

  return ch;
}

void InputStream::unread_char(char ch) {
  stream.putback(ch);
  location = saved_location;
}

void InputStream::skip_whitespaces_and_comments() {
  char ch = read_char();
  while (WHITESPACE.find(ch)) {
    if (ch == '#') {
      // `#` is a comment! Keep reading until the end of the line (include the
      // case "", the end-of-file)
      while (read_char() != '\r' && read_char() != '\n' &&
             read_char() != '\0') {
      }
    }
    ch = read_char();
    if (ch == '\0')
      return;
  }

  unread_char(ch);
}
