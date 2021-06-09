#include "scene_file.h"

void InputStream::_update_pos(char ch) {}
char InputStream::read_char() {
  char ch;
  return ch;
}
void InputStream::unread_char(char ch) {}
void InputStream::skip_whitespaces_and_comments() {}
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