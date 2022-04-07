#include "Ray.h"
#include <iostream>
Ray::Ray(const Point& origin, const Vector3d& direction) : _origin(origin), _direction(direction) {}
Ray::Ray(const Point& origin, const Vector3d& direction, const Pixel& pixel_indices) : _origin(origin), _direction(direction) {}

bool Ray::intersect(const SceneObject& obj) {
    std::array<double,3> coeffs = intersect_coeffs(obj);

    return intersect(obj,coeffs);
    
}

bool Ray::intersect(const SceneObject& obj, const std::array<double,3>& coeffs) {
    if (coeffs[1]*coeffs[1]-4*coeffs[0]*coeffs[2]<0 || coeffs[0] == 0) {
        return false;
    } else {
        return true;
    }
}

std::array<double,3> Ray::intersect_coeffs(const SceneObject& obj) {
    std::array<double,3> coeffs;

    Point new_ori = _origin-obj.centre;
    coeffs[0] = _direction.dot(_direction);
    coeffs[1] = 2*new_ori.dot(_direction);
    coeffs[2] = new_ori.dot(new_ori) - pow(obj.radius,2);
    return coeffs;
}

std::pair<bool,double> Ray::intersection(const SceneObject& obj) {
    
    Vector3d l_vec = obj.centre-_origin;
    double s = l_vec.dot(_direction);
    double l_squared = l_vec.dot(l_vec);
    double rad_squared = obj.radius*obj.radius;
    
    if (s<0 && l_squared > rad_squared) {
        return std::make_pair(false,0.0);
    }

    double m_squared = l_squared - s*s;
    // std::cout << "Do I make it here?\n";
    if (m_squared > rad_squared) {
        return std::make_pair(false, 0.0);
    }
    double q = sqrt(rad_squared-m_squared);
    double t = 0;
    if (l_squared > rad_squared) {
        t = s-q;
    } else {
        t = s + q;
    }
    
    return std::make_pair(true, t);
}

std::tuple<bool,SceneObject,double> Ray::intersection(const std::vector<SceneObject>& objs) {
    bool found_intersect = false;
    double lMin = 0.0;

    std::tuple<bool,SceneObject,double> ret = std::make_tuple(false,SceneObject(),0.0);
    
    for (SceneObject obj : objs) {
        std::pair<bool, double> check_intersect  = intersection(obj);
        
        if (check_intersect.first && check_intersect.second>0.0001 && (!found_intersect || check_intersect.second < lMin)) {
            // std::cout << "Intersected!\n";
            found_intersect = true;
            lMin = check_intersect.second;
            ret = std::make_tuple(check_intersect.first,obj,check_intersect.second);
        }


    }
    return ret;
}