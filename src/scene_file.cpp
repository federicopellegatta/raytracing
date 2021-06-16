#include "scene_file.h"

string WHITESPACE{" #\t\n\r"};
string symbols{'(', ')', '<', '>', ',', '[', ']', '*'};
map<string, KeywordEnum> KEYWORDS{
    {"new", KeywordEnum::NEW},
    {"material", KeywordEnum::MATERIAL},
    {"plane", KeywordEnum::PLANE},
    {"sphere", KeywordEnum::SPHERE},
    {"diffuse", KeywordEnum::DIFFUSE},
    {"specular", KeywordEnum::SPECULAR},
    {"uniform", KeywordEnum::UNIFORM},
    {"checkered", KeywordEnum::CHECKERED},
    {"image", KeywordEnum::IMAGE},
    {"identity", KeywordEnum::IDENTITY},
    {"translation", KeywordEnum::TRANSLATION},
    {"rotation_x", KeywordEnum::ROTATION_X},
    {"rotation_y", KeywordEnum::ROTATION_Y},
    {"rotation_z", KeywordEnum::ROTATION_Z},
    {"scaling", KeywordEnum::SCALING},
    {"camera", KeywordEnum::CAMERA},
    {"orthogonal", KeywordEnum::ORTHOGONAL},
    {"perspective", KeywordEnum::PERSPECTIVE},
    {"float", KeywordEnum::FLOAT},
};

void InputStream::_update_pos(const char &ch) {
  if (ch == '\0') {
    // Nothing to do
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
    if (stream.eof()) {
      ch = '\0';
    }
  }

  saved_location = location;
  _update_pos(ch);

  return ch;
}

void InputStream::unread_char(const char &ch) {
  stream.putback(ch);
  location = saved_location;
}

void InputStream::skip_whitespaces_and_comments() {
  char ch = read_char();
  while (WHITESPACE.find(ch) != string::npos) {
    if (ch == '#') {
      // `#` is a comment! Keep reading until the end of the line (include the
      // case "", the end-of-file)
      while (ch != '\r' && ch != '\n' && ch != '\0') {
        ch = read_char();
      }
    }
    ch = read_char();
    if (ch == '\0')
      return;
  }
  unread_char(ch);
}

Token InputStream::_parse_string_token(SourceLocation _location) {
  string string_token;

  while (true) {
    char ch = read_char();
    if (ch == '"') {
      break;
    }
    if (ch == '\0') {
      throw GrammarError(_location, "unterminated string");
    }
    string_token.push_back(ch);
  }

  Token token(_location);
  token.assign_string(string_token);

  return token;
}

Token InputStream::_parse_float_token(const char &first_ch,
                                      SourceLocation _location) {
  string string_token;
  string_token.push_back(first_ch);
  float value = 0.;
  while (true) {
    char ch = read_char();
    if (!isdigit(ch) && ch != '.' && ch != 'e' && ch != 'E') {
      unread_char(ch);
      break;
    }
    string_token.push_back(ch);
  }

  try {
    value = stof(string_token);
  } catch (invalid_argument) {
    throw GrammarError(_location,
                       string_token + " is an invalid floating point number");
  } catch (out_of_range &e) {
    throw GrammarError(_location, string_token + ":" + e.what());
  }

  Token token;
  token.assign_number(value);

  return token;
}

Token InputStream::_parse_keyword_or_identifier_token(
    const char &first_ch, SourceLocation _location) {
  string string_token;
  string_token.push_back(first_ch);

  while (true) {
    char ch = read_char();

    if (!isalnum(ch) && ch != '_') {
      unread_char(ch);
      break;
    }

    string_token.push_back(ch);
  }
  Token token;
  try {
    token.assign_keyword(KEYWORDS.at(string_token));
  } catch (out_of_range) {
    token.assign_identifier(string_token);
  }
  return token;
}

Token InputStream::_parse_symbol_token(const char &first_ch,
                                       SourceLocation location) {
  Token token(location);
  token.assign_symbol(first_ch);
  return token;
}

Token InputStream::read_token() {

  if (saved_token.location.col_num != 0 || saved_token.location.col_num != 0) {
    Token result = saved_token;
    saved_token = Token();
    return saved_token;
  }

  skip_whitespaces_and_comments();

  // At this point we're sure that ch does *not* contain a whitespace character
  char ch = read_char();

  if (ch == '\0') {
    // No more characters in the file, so return a StopToken
    Token _stopToken;
    _stopToken.assign_stop();
    return _stopToken;
  }

  // At this point we must check what kind of token begins with the "ch"
  // character (which has been put back in the stream with self.unread_char).
  // First, we save the position in the stream
  SourceLocation token_location = location;
  if (symbols.find(ch) != string::npos) {
    // One-character symbol, like '(' or ','
    return _parse_symbol_token(ch, token_location);
  } else if (ch == '"') {
    // A literal string (used for file names)
    return _parse_string_token(token_location);
  } else if (isdigit(ch) || ch == '+' || ch == '-' || ch == '.') {
    // A floating-point number
    return _parse_float_token(ch, token_location);
  } else if (isalpha(ch) || ch == '_') {
    // Since it begins with an alphabetic character, it must either be a keyword
    // or a identifier
    return _parse_keyword_or_identifier_token(ch, token_location);
  } else {
    // We got some weird character, like '@` or `&`
    throw GrammarError(location, ch + ": invalid character");
  }
}

void InputStream::unread_token(Token _token) {
  assert(saved_token.location.col_num != 0 ||
         saved_token.location.col_num != 0);
  saved_token = _token;
}

void InputStream::expect_symbol(InputStream input_file, string str) {
  Token token = input_file.read_token();
  if (token.type == TokenType::SYMBOL)
    throw(GrammarError(token.location, "got '" + string{token.value.symbol} +
                                           "' instead of '" + str + "'"));
}

KeywordEnum InputStream::expect_keywords(InputStream input_file,
                                         vector<KeywordEnum> keywords) {
  Token token = input_file.read_token();

  if (token.type != TokenType::KEYWORD) {
    throw(GrammarError(token.location, "expected a keyword instead of " +
                                           string{token.value.symbol}));
  }

  if (find(keywords.begin(), keywords.end(), token.value.keyword) ==
      keywords.end()) {
    throw(GrammarError(
        token.location,
        "expected one of the keywords in `keywords` instead of '"));
    //+ string{token.value.keyword} + "'"));
  }
  return token.value.keyword;
}