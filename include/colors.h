// Color header file
struct Color {
public:
  float r, b, g;

  Color(float, float, float);
  ~Color();

  Color operator+(Color, Color);
  Color operator*(float, Color);
  Color operator*(Color, float);
  Color operator*(Color, Color);

  bool is_close(Color);

private:
  bool are_close(float, float);
};
