/*
 * Copyright (c) 2021 Simone Pirota, Federico Pellegatta
 *
 * This file is part of raytracer.
 *
 * raytracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * raytracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with raytracer.  If not, see <https://www.gnu.org/licenses/>.
 */

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
  assert(stream.read_token().type == TokenType::STOP);
}

void test_parser() {
  stringstream sstr(
      "\nfloat clock(150)\n\nmaterial "
      "sky_material(\n\tdiffuse(uniform(<0, 0, 0>)),\n\tuniform(<0.7, 0.5, "
      "1>)\n)\n\n# Here is a comment\n\nmaterial "
      "ground_material(\n\tdiffuse(checkered(<0.3, "
      "0.5, 0.1>,\n\t\t\t  <0.1, 0.2, 0.5>, 4)),\n\tuniform(<0, 0, "
      "0>)\n)\n\nmaterial sphere_material(\n\tspecular(uniform(<0.5, 0.5, "
      "0.5>)),\n\tuniform(<0, 0, 0>)\n)\n\nplane (sky_material, "
      "translation([0, 0, 100]) * rotation_y(clock))\nplane (ground_material, "
      "identity)\n\nsphere(sphere_material, translation([0, 0, "
      "1]))\n\ncamera(perspective, rotation_z(30) * translation([-4, 0, 1]), "
      "1.0, 2.0)\n");

  fmt::print(sstr.str());
  InputStream stream(sstr);
  map<string, float> vars;
  Scene scene = stream.parse_scene(vars);

  // Check that the float variables are ok
  assert(scene.float_variables.size() == 1);
  assert(scene.float_variables.find("clock") != scene.float_variables.end());
  assert(scene.float_variables.at("clock") == 150.0);

  // Check that the materials are ok
  assert(scene.materials.size() == 3);
  assert(scene.materials.find("sphere_material") != scene.materials.end());
  assert(scene.materials.find("sky_material") != scene.materials.end());
  assert(scene.materials.find("ground_material") != scene.materials.end());

  // Check if the shapes are ok
  assert(scene.world.shapes.size() == 3);
  assert(scene.world.shapes.at(0)->transformation.is_close(
      translation(Vec(0, 0, 100)) * rotation_y(150.0)));
  assert(scene.world.shapes.at(1)->transformation.is_close(Transformation()));
  assert(scene.world.shapes.at(2)->transformation.is_close(
      translation(Vec(0, 0, 1))));

  // Check if the camera is ok
  assert(scene.camera->transformation.is_close(rotation_z(30) *
                                               translation(Vec(-4, 0, 1))));
  assert(are_close(scene.camera->aspect_ratio, 1.0));
}

void test_parser_undefined_material() {
  stringstream sstr("\nplane(this_material_does_not_exist, identity)\n");
  InputStream stream(sstr);
  map<string, float> vars;
  try {
    stream.parse_scene(vars);
    fmt::print("The code did not throw an exception");
    assert(false);
  } catch (GrammarError &e) {
    fmt::print("{}\n", e.what());
    assert(true);
  }
}

void test_parser_double_camera() {
  stringstream sstr(
      "\ncamera(perspective, rotation_z(30) * translation([-4, 0, 1]), 1.0, "
      "1.0)\ncamera(orthogonal, identity, 1.0, 1.0)\n");
  InputStream stream(sstr);
  map<string, float> vars;
  try {
    stream.parse_scene(vars);
    fmt::print("The code did not throw an exception");
    assert(false);
  } catch (GrammarError &e) {
    fmt::print("{}\n", e.what());
    assert(true);
  }
}

int main() {
  test_input_file();
  test_lexer();
  test_parser();
  test_parser_undefined_material();
  test_parser_double_camera();
  return 0;
}