#ifndef SCENE_FILE_H
#define SCENE_FILE_H

#include "HdrImage.h"
#include "camera.h"
#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "shapes.h"
#include "world.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

/**
 * @brief A specific position in a source file
 *
 * This class has the following fields:
 * @param file_name: the name of the file, or the empty string if there is
 * no file associated with this location (e.g., because the source code was
 * provided as a memory stream, or through a network connection)
 * @param line_num: number of the line (starting from 1)
 * @param col_num: number of the column (starting from 1)
 *
 */
struct SourceLocation {

  string file_name;
  int line_num;
  int col_num;

  /**
   * @brief Construct a new Source Location object: a specific position in a
   * source file
   *
   * @param file_name: the name of the file, or the empty string if there is no
   * file associated with this location (e.g., because the source code was
   * provided as a memory stream, or through a network connection)
   * @param line_num: number of the line (starting from 1)
   * @param col_num: number of the column (starting from 1)
   */
  SourceLocation(string _file_name = "", int _line_num = 0, int _col_num = 0)
      : file_name{_file_name} {
    line_num = _line_num;
    col_num = _col_num;
  }
};

/**
 * @brief Struct representing an error found by the lexer/parser while reading a
 * scene file
 *
 */
struct GrammarError : public exception {
  SourceLocation location;
  string message;

  /**
   * @brief Construct a new Grammar Error object
   *
   * @param _location: position of the error
   * @param _message: the error message
   */
  GrammarError(SourceLocation _location, const string &_message)
      : location{_location}, message{_message} {}

  const char *what() const throw() { return message.c_str(); }
};

/**
 * @brief All possible keywords accepted
 *
 */
enum class KeywordEnum {
  NEW,
  MATERIAL,
  PLANE,
  SPHERE,
  DIFFUSE,
  SPECULAR,
  UNIFORM,
  CHECKERED,
  IMAGE,
  IDENTITY,
  TRANSLATION,
  ROTATION_X,
  ROTATION_Y,
  ROTATION_Z,
  SCALING,
  CAMERA,
  ORTHOGONAL,
  PERSPECTIVE,
  FLOAT
};

/**
 * @brief All possible types of tokens
 *
 */
enum class TokenType {
  LITERAL_NUMBER,
  LITERAL_STRING,
  SYMBOL,
  KEYWORD,
  IDENTIFIER,
  STOP
};

/**
 * @brief Union representing the values of each TokenType
 *
 */
union TokenValue {
  float number;
  string str;
  KeywordEnum keyword;
  char symbol;

  // The default constructor and destructor are *mandatory* for unions to
  // be used in structs/classes
  TokenValue() : str{""} {}
  TokenValue(const TokenValue &t) : str{t.str} {}
  ~TokenValue() {}
};

/**
 * @brief Struct representing a Token, with its type, value and poisition
 *
 */
struct Token {
  TokenType type;   // The "tag"
  TokenValue value; // The "union"
  SourceLocation location;

  /**
   * @brief Construct a new Token object
   *
   * @param _location
   */
  Token(SourceLocation _location = SourceLocation())
      : type(TokenType::LITERAL_NUMBER), location{_location} {}

  Token(const Token &t) : type{t.type}, location{t.location} {
    switch (t.type) {
    case TokenType::IDENTIFIER:
      value.str = t.value.str;
      break;
    case TokenType::KEYWORD:
      value.keyword = t.value.keyword;
      break;
    case TokenType::LITERAL_NUMBER:
      value.number = t.value.number;
      break;
    case TokenType::LITERAL_STRING:
      value.str = t.value.str;
      break;
    case TokenType::SYMBOL:
      value.symbol = t.value.symbol;
      break;
    default:
      break;
    }
  }

  Token operator=(const Token &t) {
    Token token{t};
    return token;
  }

  /**
   * @brief Define the Token as a number and assign its value
   *
   * @param val
   */
  void assign_number(float val) {
    type = TokenType::LITERAL_NUMBER;
    value.number = val;
  }

  /**
   * @brief Define the Token as a string and assign its value
   *
   * @param s
   */
  void assign_string(const string &s) {
    type = TokenType::LITERAL_STRING;
    value.str = s;
  }

  /**
   * @brief Define the Token as a keyword and assign its value
   *
   * @param k
   */
  void assign_keyword(const KeywordEnum &k) {
    type = TokenType::KEYWORD;
    value.keyword = k;
  }

  /**
   * @brief Define the Token as a symbol and assign its value
   *
   * @param c
   */
  void assign_symbol(const char &c) {
    type = TokenType::SYMBOL;
    value.symbol = c;
  }

  /**
   * @brief Define the Token as an identifier and assign its value
   *
   * @param id
   */
  void assign_identifier(const string &id) {
    type = TokenType::IDENTIFIER;
    value.str = id;
  }

  void assign_stop() { type = TokenType::STOP; }
};

/**
 * @brief A scene read from a scene file
 *
 */
struct Scene {
  map<string, Material> materials;
  World world;
  shared_ptr<Camera> camera;
  map<string, float> float_variables;
  vector<string> overridden_variables;

  Scene();
};

struct InputStream {
public:
  istream &stream;
  SourceLocation location;
  SourceLocation saved_location;
  int tabulations;
  char saved_char = '\0'; // '\0' is the null char
  Token saved_token;

  /**
   * @brief Construct a new Input Stream object
   *
   * @param stream
   * @param location
   * @param tabulations
   */
  InputStream(istream &_stream, string filename = "", int _tabulations = 4)
      : stream{_stream}, location{filename, 1, 1}, saved_token{} {
    tabulations = _tabulations;
  }

  /**
   * @brief Read a new character from the stream
   *
   * @return char
   */
  char read_char();
  /**
   * @brief Push a character back to the stream
   *
   * @param ch `Char` to push back
   */
  void unread_char(const char &ch);

  /**
   * @brief Keep reading characters until a non-whitespace character is found
   *
   */
  void skip_whitespaces_and_comments();
  Token read_token();

  /**
   * @brief Make as if `token` were never read from `input_file
   *
   * @param token
   */
  void unread_token(const Token &);

private:
  /**
   * @brief Update `location` after having read `ch` from the stream
   *
   * @param ch `char`
   */
  void _update_pos(const char &ch);
  Token _parse_string_token(SourceLocation);
  Token _parse_float_token(const char &, SourceLocation);
  Token _parse_keyword_or_identifier_token(const char &, SourceLocation);
  Token _parse_symbol_token(const char &, SourceLocation);

  /**
   * @brief Read a token from `stream` and check that it matches `symbol`
   *
   * @param ch
   */
  void expect_symbol(const char &ch);

  /**
   * @brief Read a token from `stream` and check that it is one of the
   * keywords in `keywords
   *
   * @param keywords
   * @return KeywordEnum
   */
  KeywordEnum expect_keywords(const vector<KeywordEnum> &keywords);

  /**
   * @brief Read a token from `stream` and check that it is either a literal
   * number or a variable in `scene` and return the number as a `float`
   *
   * @param scene
   * @return float
   */
  float expect_number(const Scene &scene);

  /**
   * @brief Read a token from `stream` and check that it is a literal
   string
   *
   * @return Return the value of the string (a ``str``).
   */
  string expect_string();

  /**
   * @brief ead a token from `stream` and check that it is an identifier.
   *
   * @return Return the name of the identifier.
   */
  string expect_identifier();
  Vec _parse_vector(const Scene &);
  Color _parse_color(const Scene &);
  shared_ptr<Pigment> _parse_pigment(const Scene &);
};

#endif