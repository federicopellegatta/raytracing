#ifndef SCENE_FILE_H
#define SCENE_FILE_H

#include "HdrImage.h"
#include "camera.h"
#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "shapes.h"
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

/**
 * @brief A specific position in a source file
 *
 * This class has the following fields:
 * @param file_name: the name of the file, or the empty string if there is no
 * file associated with this location (e.g., because the source code was
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
  SourceLocation(string _file_name = "", int _line_num = 0, int _col_num = 0) {
    file_name = _file_name;
    line_num = _line_num;
    col_num = _col_num;
  }
};

/**
 * @brief Struct representing an error found by the lexer/parser while reading a
 * scene file
 *
 */
struct GrammarError {
  SourceLocation location;
  string message;

  /**
   * @brief Construct a new Grammar Error object
   *
   * @param _location: position of the error
   * @param _message: the error message
   */
  GrammarError(SourceLocation _location = SourceLocation(),
               string _message = "")
      : location{_location} {
    message = _message;
  }
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
  IDENTIFIER
};

/**
 * @brief Union representing the values of each TokenType
 *
 */
union TokenValue {
  float number;
  string str;
  string keyword;
  string identifier;
  char symbol;

  // The default constructor and destructor are *mandatory* for unions to
  // be used in structs/classes
  TokenValue() { number = 0.; }
  ~TokenValue() {}
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
 * @brief Dictionary used to map strings in file to `KeywordEnum` type
 *
 */
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

/**
 * @brief All symbols recognized as such by the lexer/parser
 *
 */
char symbols[9] = {'(', ')', '<', '>', ',', '"', '[', ']', '*'};

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
  void assign_keyword(const string &k) {
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
    value.identifier = id;
  }
};

struct InputStream {
public:
  istream stream;
  SourceLocation location;
  SourceLocation saved_location;
  Token saved_token;
  char saved_char;
  int tabulation;

  InputStream(istream &_stream, string file_name = "", int _tabulation = 8)
      : stream(_stream) {
    location = SourceLocation(file_name, 1, 1);
    saved_location = location;
    saved_char = ' ';
    tabulation = _tabulation;
  }

  char read_char();
  void unread_char(char);
  void skip_whitespaces_and_comments();
  Token read_token();
  void unread_token(Token);

private:
  void _update_pos(char);
  Token _parse_string_token(SourceLocation);
  Token _parse_float_token(char, SourceLocation);
  Token _parse_keyword_or_identifier_token(char, SourceLocation);
};
#endif