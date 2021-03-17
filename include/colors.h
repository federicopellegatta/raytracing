// Color header file
struct Color {
public:
  float r, b, g;

  Color() {
    r = 0;
    b = 0;
    g = 0;
  }
  Color(float, float, float);
  ~Color();

  Color operator+(Color);
  Color operator*(float);
  // Color operator*(Color, float);
  Color operator*(Color);

  bool is_close(Color);
  void print();

private:
  bool are_close(float, float);
};
