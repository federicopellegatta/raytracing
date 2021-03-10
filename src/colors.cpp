// src/colors.cpp

#include "../include/colors.h"
#include <iostream>

using namespace std;

Color::Color(float r, float g, float b) {
  Color.r = r;
  Color.g = g;
  Color.b = b;
}

Color::~Color(){};

Color::operator+(Color a, Color b) {
  return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}

Color::operator*(float a, Color b) { return Color(a * b.r, a * b.g, a * b.b); }

Color::operator*(Color b, float a) { return Color(a * b.r, a * b.g, a * b.b); }

Color::operator*(Color a, Color b) {
  return Color(a.r * b.r, a.g * b.g, a.b * b.b);
}
