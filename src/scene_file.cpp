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

void InputStream::unread_token(const Token &_token) {
  assert(saved_token.location.col_num == 0 &&
         saved_token.location.line_num == 0);
  saved_token = _token;
}

void InputStream::expect_symbol(const char &ch) {
  Token token = read_token();
  if (token.type == TokenType::SYMBOL) {
    throw(GrammarError(token.location, "got '" + string{token.value.symbol} +
                                           "' instead of '" + ch + "'"));
  }
}

KeywordEnum InputStream::expect_keywords(const vector<KeywordEnum> &keywords) {
  Token token = read_token();

  if (token.type != TokenType::KEYWORD) {
    throw(GrammarError(token.location, "expected a keyword instead of " +
                                           string{token.value.symbol}));
  }

  if (find(keywords.begin(), keywords.end(), token.value.keyword) ==
      keywords.end()) {
    throw(GrammarError(token.location,
                       "expected one of the keywords in `keywords`'"));
  }
  return token.value.keyword;
}

float InputStream::expect_number(const Scene &_scene) {
  Token token = read_token();
  if (token.type == TokenType::LITERAL_NUMBER) {
    return token.value.number;
  } else if (token.type == TokenType::IDENTIFIER) {
    string variable_name = token.value.str;
    if (_scene.float_variables.find(variable_name) ==
        _scene.float_variables.end()) {
      throw(GrammarError(token.location,
                         "unknown variable '" + token.value.str + "'"));
    }
    return _scene.float_variables.at(variable_name);
  } else {
    throw(GrammarError(token.location,
                       "got '" + token.value.str + "' instead of a number"));
  }
}

string InputStream::expect_string() {
  Token token = read_token();
  // if (token.type != TokenType::LITERAL_STRING) {
  //  throw(GrammarError(token.location,
  //                     "got '" + token.value.str + "' instead of a string"));
  //}
  switch (token.type) {
  case TokenType::IDENTIFIER:
    throw(GrammarError(token.location,
                       "Got '" + token.value.str + "' instead of a string"));
    break;
  case TokenType::KEYWORD:
    throw(GrammarError(token.location, "Got a keyword instead of a string."));
    break;
  case TokenType::LITERAL_NUMBER:
    throw(GrammarError(token.location, "Got a number instead of a string"));
    break;
  case TokenType::STOP:
    throw(GrammarError(token.location, "Got a STOP token instead of a string"));
    break;
  case TokenType::SYMBOL:
    throw(GrammarError(token.location, "Got '" + string{token.value.symbol} +
                                           "' instead of a string"));
    break;
  }
  return token.value.str;
}

string InputStream::expect_identifier() {
  Token token = read_token();
  // if (token.type != TokenType::IDENTIFIER)
  //  throw(GrammarError(token.location, "got '" + token.value.str +
  //                                         "' instead of an identifier"));
  switch (token.type) {
  case TokenType::LITERAL_STRING:
    throw(GrammarError(token.location, "Got '" + token.value.str +
                                           "' instead of a identifier"));
    break;
  case TokenType::KEYWORD:
    throw(
        GrammarError(token.location, "Got a keyword instead of a identifier"));
    break;
  case TokenType::LITERAL_NUMBER:
    throw(GrammarError(token.location, "Got a number instead of a identifier"));
    break;
  case TokenType::STOP:
    throw(GrammarError(token.location,
                       "Got a STOP token instead of a identifier"));
    break;
  case TokenType::SYMBOL:
    throw(GrammarError(token.location, "Got '" + string{token.value.symbol} +
                                           "' instead of a identifier"));
    break;
  }
  return token.value.str;
}

Vec InputStream::_parse_vector(const Scene &_scene) {
  expect_symbol('[');
  float x = expect_number(_scene);
  expect_symbol(',');
  float y = expect_number(_scene);
  expect_symbol(',');
  float z = expect_number(_scene);
  expect_symbol(']');

  return Vec{x, y, z};
}

Color InputStream::_parse_color(const Scene &_scene) {
  expect_symbol('<');
  float red = expect_number(_scene);
  expect_symbol(',');
  float green = expect_number(_scene);
  expect_symbol(',');
  float blue = expect_number(_scene);
  expect_symbol('>');

  return Color{red, green, blue};
}

shared_ptr<Pigment> InputStream::_parse_pigment(const Scene &_scene) {
  vector<KeywordEnum> expected_keys{KeywordEnum::UNIFORM,
                                    KeywordEnum::CHECKERED, KeywordEnum::IMAGE};
  KeywordEnum keyword = expect_keywords(expected_keys);
  shared_ptr<Pigment> result;
  expect_symbol('(');
  if (keyword == KeywordEnum::UNIFORM) {
    Color _color = _parse_color(_scene);
    result = make_shared<UniformPigment>(_color);
  } else if (keyword == KeywordEnum::CHECKERED) {
    Color _color1 = _parse_color(_scene);
    expect_symbol(',');
    Color _color2 = _parse_color(_scene);
    expect_symbol(',');
    int num_of_steps = static_cast<int>(expect_number(_scene));
    result = make_shared<CheckeredPigment>(_color1, _color2, num_of_steps);
  } else if (keyword == KeywordEnum::IMAGE) {
    string filename = expect_string();
    HdrImage image{filename};
    result = make_shared<ImagePigment>(image);
  } else {
    fmt::print("ERROR: {} is not a valid pigment type!", keyword);
    assert(false);
  }
  expect_symbol(')');
  return result;
}

shared_ptr<BRDF> InputStream::_parse_brdf(const Scene &_scene) {
  vector<KeywordEnum> brdf_keyword{KeywordEnum::DIFFUSE, KeywordEnum::SPECULAR};
  KeywordEnum keyword = expect_keywords(brdf_keyword);

  shared_ptr<BRDF> result;
  expect_symbol('(');
  shared_ptr<Pigment> pigment = _parse_pigment(_scene);
  expect_symbol(')');

  if (keyword == KeywordEnum::DIFFUSE)
    result = make_shared<DiffusiveBRDF>(pigment);
  else if (keyword == KeywordEnum::SPECULAR)
    result = make_shared<SpecularBRDF>(pigment);

  return result;
}

tuple<string, Material> InputStream::_parse_material(const Scene &_scene) {
  string name = expect_identifier();
  expect_symbol('(');
  shared_ptr<BRDF> brdf = _parse_brdf(_scene);
  expect_symbol(',');
  shared_ptr<Pigment> emitted_radiance = _parse_pigment(_scene);
  expect_symbol(')');

  return tuple<string, Material>{name, Material{brdf, emitted_radiance}};
}

Transformation InputStream::_parse_transformation(const Scene &_scene) {
  Transformation result;
  vector<KeywordEnum> transformation_keyword{
      KeywordEnum::IDENTITY,   KeywordEnum::TRANSLATION,
      KeywordEnum::ROTATION_X, KeywordEnum::ROTATION_Y,
      KeywordEnum::ROTATION_Z, KeywordEnum::SCALING,
  };

  while (true) {
    KeywordEnum keyword = expect_keywords(transformation_keyword);

    switch (keyword) {
    case KeywordEnum::IDENTITY:
      break; // Do nothing (this is a primitive form of optimization!)

    case KeywordEnum::TRANSLATION:
      expect_symbol('(');
      result = result * translation(_parse_vector(_scene));
      expect_symbol(')');
      break;

    case KeywordEnum::ROTATION_X:
      expect_symbol('(');
      result = result * rotation_x(expect_number(_scene));
      expect_symbol(')');
      break;

    case KeywordEnum::ROTATION_Y:
      expect_symbol('(');
      result = result * rotation_y(expect_number(_scene));
      expect_symbol(')');
      break;

    case KeywordEnum::ROTATION_Z:
      expect_symbol('(');
      result = result * rotation_z(expect_number(_scene));
      expect_symbol(')');
      break;

    case KeywordEnum::SCALING:
      expect_symbol('(');
      result = result * scaling(_parse_vector(_scene));
      expect_symbol(')');
      break;
    }

    Token next_keyword = read_token();

    if (next_keyword.type == TokenType::SYMBOL ||
        next_keyword.value.symbol != '*') {
      unread_token(next_keyword);
      break;
    }
  }
  return result;
}

Sphere InputStream::_parse_sphere(const Scene &_scene) {
  expect_symbol('(');
  string material_name = expect_identifier();

  if (_scene.materials.find(material_name) == _scene.materials.end()) {
    // We raise the exception here because input_file is pointing to the end of
    // the wrong identifier
    throw(GrammarError(location, "unknown material '" + material_name + "'"));
  }
  expect_symbol(',');
  Transformation transformation = _parse_transformation(_scene);
  expect_symbol(')');

  return Sphere(transformation, _scene.materials.at(material_name));
}

Plane InputStream::_parse_plane(const Scene &_scene) {
  expect_symbol('(');
  string material_name = expect_identifier();

  if (_scene.materials.find(material_name) == _scene.materials.end()) {
    // We raise the exception here because input_file is pointing to the end of
    // the wrong identifier
    throw(GrammarError(location, "unknown material '" + material_name + "'"));
  }
  expect_symbol(',');
  Transformation transformation = _parse_transformation(_scene);
  expect_symbol(')');

  return Plane(transformation, _scene.materials.at(material_name));
}

shared_ptr<Camera> InputStream::_parse_camera(const Scene &_scene) {
  expect_symbol('(');
  vector<KeywordEnum> camera_keyword{KeywordEnum::PERSPECTIVE,
                                     KeywordEnum::ORTHOGONAL};
  KeywordEnum type_keyword = expect_keywords(camera_keyword);
  expect_symbol(',');
  Transformation transformation = _parse_transformation(_scene);
  expect_symbol(',');
  float aspect_ratio = expect_number(_scene);
  expect_symbol(',');
  float distance = expect_number(_scene);
  expect_symbol(')');

  shared_ptr<Camera> result;
  if (type_keyword == KeywordEnum::PERSPECTIVE)
    result =
        make_shared<PerspectiveCamera>(distance, aspect_ratio, transformation);
  else if (type_keyword == KeywordEnum::ORTHOGONAL)
    result = make_shared<OrthogonalCamera>(aspect_ratio, transformation);

  return result;
}

Scene InputStream::_parse_scene(const map<string, float> &variables) {
  Scene _scene;
  _scene.float_variables = variables;
  for (auto const &key : variables) {
    _scene.overridden_variables.push_back(key.first);
  }

  while (true) {
    Token what = read_token();

    if (what.type == TokenType::STOP) {
      break;
    }
    if (what.type != TokenType::KEYWORD) {
      throw GrammarError(what.location, "ERROR: expected keyword, not found.");
    }

    switch (what.value.keyword) {
    case KeywordEnum::FLOAT: {
      string variable_name = expect_identifier();
      SourceLocation variable_loc = location;

      expect_symbol('(');
      float variable_value = expect_number(_scene);
      expect_symbol(')');

      auto it = find(_scene.overridden_variables.begin(),
                     _scene.overridden_variables.end(), variable_name);
      if (_scene.float_variables.find(variable_name) !=
              _scene.float_variables.end() &&
          it == _scene.overridden_variables.end()) {
        throw GrammarError(variable_loc, "variable '" + variable_name +
                                             "' cannot be redefined");
      }
      if (it == _scene.overridden_variables.end()) {
        _scene.float_variables[variable_name] = variable_value;
      }
    } break;
    case KeywordEnum::SPHERE:
      _scene.world.add(make_shared<Sphere>(_parse_sphere(_scene)));
      break;
    case KeywordEnum::PLANE:
      _scene.world.add(make_shared<Plane>(_parse_plane(_scene)));
      break;
    case KeywordEnum::CAMERA:
      if (_scene.camera) {
        throw GrammarError(what.location, "Cannot define more than one camera");
      } else {
        _scene.camera = _parse_camera(_scene);
      }
      break;
    case KeywordEnum::MATERIAL: {
      tuple<string, Material> material = _parse_material(_scene);
      _scene.materials[get<string>(material)] = get<Material>(material);
    } break;
    }
  }
  return _scene;
}