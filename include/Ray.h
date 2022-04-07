#pragma once
#include "types.h"
#include "SceneObject.h"
#include <array>
#include <vector>
class Ray {
    public:

        double intensity = 1.0;

        Ray(const Point& origin, const Vector3d& direction);
        Ray(const Point& origin, const Vector3d& direction, const Pixel& pixel_indices);

        bool intersect(const SceneObject& obj);
        bool intersect(const SceneObject& obj, const std::array<double,3>& coeffs);
        std::array<double,3> intersect_coeffs(const SceneObject& obj);
        std::pair<bool, double> intersection(const SceneObject& obj);
        std::tuple<bool, SceneObject, double> intersection(const std::vector<SceneObject>& objs);

        inline Point sample(const double& t_val) const {return (_origin+t_val*_direction);}

        inline Vector3d get_direction() const {return _direction;}
        inline Point get_origin() const {return _origin;}

 

    private:
        Point _origin;
        Vector3d _direction;

};