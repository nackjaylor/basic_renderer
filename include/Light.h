#pragma once

#include "types.h"
#include <opencv2/core.hpp>


class Light {
    public:

        cv::Vec3f colour = cv::Vec3f(1.0,1.0,1.0);
        Light(const Point& location, const Vector3d& direction);
        Light(const Point& location, const Vector3d& direction, const cv::Vec3f& colour);

        inline Point get_location() const {return _location;}
        inline Vector3d get_direction() const {return _direction;}


    private:

        Point _location;
        Vector3d _direction;
        

};