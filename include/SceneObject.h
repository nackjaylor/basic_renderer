#pragma once
#include "types.h"
#include <opencv2/core.hpp>
class SceneObject {

    public:

        double radius = 0.5;
        double n_reff = 1.5;
        cv::Vec3f solid_colour = cv::Vec3f(0.5,0,0);
        double alpha = 1;
        Point centre = Point(0,0,0);
        double reflection_factor = 0.60;
        double spec_value = 1;
        double spec_power = 100;
        cv::Mat texture_map;
        
        SceneObject();
        SceneObject(const double& radius_, const double& alpha_, const Point& centre_);
        SceneObject(const double& radius_, const double& alpha_, const cv::Vec3f& colour_, const Point& centre_);
        SceneObject(const double& radius_, const double& alpha_, const cv::Mat& text_map, const Point& centre_);

        Vector3d calc_normal(const Point& surface_point) const;
        cv::Vec3f colour(const Point& surface_point) const;

    private:
        bool map_flag = false;

};