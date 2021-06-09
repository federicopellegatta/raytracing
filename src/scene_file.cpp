#include "scene_file.h"

void InputStream::_update_pos(char ch) {
  if (ch == '\0') {
    // Notihing to do
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
  _update_pos(ch);

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

Token InputStream::_parse_string_token(SourceLocation _location) {
  return Token();
}
Token InputStream::_parse_float_token(char first_ch, SourceLocation _location) {
  return Token();
}
Token InputStream::_parse_keyword_or_identifier_token(
    char first_ch, SourceLocation _location) {
  return Token();
}
Token InputStream::read_token() { return Token(); }
void InputStream::unread_token(Token _token) {}