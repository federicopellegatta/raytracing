#ifndef SCENE_FILE_H
#define SCENE_FILE_H

#include "HdrImage.h"
#include "camera.h"
#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "shapes.h"
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

  string file_name = "";
  int line_num = 0;
  int col_num = 0;

  SourceLocation(){};

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
  SourceLocation(string file_name, int line_num, int col_num);
};

enum class TokenType {
  LITERAL_NUMBER,
  LITERAL_STRING,
  SYMBOL,
  KEYWORD,
  IDENTIFIER,
};

// The sum type.
union TokenValue {
  float number;
  std::string string;
  std::string keyword;
  std::string identifier;
  char symbol;

  // The default constructor and destructor are *mandatory* for unions to
  // be used in structs/classes
  TokenValue() : number(0.0) {}
  ~TokenValue() {}
};

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

char symbols[9] = {'(', ')', '<', '>', ',', '"', '[', ']', '*'};

// Here is the "Token" type! We just combine `TokenType` and `TokenValue`
// in a product type, which implements a proper "tagged union".
struct Token {
  TokenType type;   // The "tag"
  TokenValue value; // The "union"

  Token() : type(TokenType::LITERAL_NUMBER) {}

  void assign_number(float val) {
    type = TokenType::LITERAL_NUMBER;
    value.number = val;
  }

  void assign_string(const std::string &s) {
    type = TokenType::LITERAL_STRING;
    value.string = s;
  }

  void assign_keyword(const std::string &k) {
    type = TokenType::KEYWORD;
    value.keyword = k;
  }

  void assign_symbol(const char &s) {
    type = TokenType::SYMBOL;
    value.symbol = s;
  }

  void assign_identifier(const std::string &id) {
    type = TokenType::IDENTIFIER;
    value.identifier = id;
  }
};

struct InputStream {
  istream stream;
  SourceLocation location;
  char saved_char;
  int tabulation;
  Token saved_token;

  /*InputStream(ifstream &_stream, string file_name = "", int _tabulation = 8) {
    stream = _stream;
    location = SourceLocation(file_name, 1, 1);
    saved_char = ' ';
    SourceLocation saved_location = location;
    tabulation = _tabulation;

    // saved_token:
    // Union[Token, None] = None
  }*/
};
#endif