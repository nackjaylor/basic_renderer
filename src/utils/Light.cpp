#include "Light.h"

Light::Light(const Point& location, const Vector3d& direction) : _location(location), _direction(direction) {};

Light::Light(const Point& location, const Vector3d& direction, const cv::Vec3f& _colour) : _location(location), _direction(direction), colour(_colour) {};