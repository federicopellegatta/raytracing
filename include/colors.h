// include/colors.h

struct Color {
public:
  float r, b, g;

  Color(float, float, float);
  ~Color();

  Color operator+(Color, Color);
  Color operator*(float, Color);
  Color operator*(Color, float);
  Color operator*(Color, Color);

  bool are_close(Color, Color);

private:
  bool are_close(float a, float b, float epsilon);
};
