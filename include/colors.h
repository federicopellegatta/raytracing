// Color header file
struct Color {
public:
  float r, b, g;

  Color(float, float, float);
  ~Color();

  Color operator+(Color);
  Color operator*(float);
  // Color operator*(Color, float);
  Color operator*(Color);

  bool is_close(Color);

private:
  bool are_close(float, float);
};
