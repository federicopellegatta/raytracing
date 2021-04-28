#include "camera.h"

using namespace std;

OrtogonalCamera::OrtogonalCamera(
    float _aspect_ratio = 1.0,
    Transformation _transformation = Transformation())
    : aspect_ratio{_aspect_ratio}, transformation{_transformation} {}

Ray OrtogonalCamera::fire_ray(float _u, float _v) {}