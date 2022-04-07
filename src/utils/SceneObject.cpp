#include "SceneObject.h"

#include "CoordTools.h"


SceneObject::SceneObject() {}

SceneObject::SceneObject(const double& radius_, const double& alpha_, const Point& centre_) : radius(radius_), alpha(alpha_), centre(centre_) {}

SceneObject::SceneObject(const double& radius_, const double& alpha_, const cv::Vec3f& colour_, const Point& centre_) : radius(radius_), alpha(alpha_), centre(centre_), solid_colour(colour_) {}


SceneObject::SceneObject(const double& radius_, const double& alpha_, const cv::Mat& text_map, const Point& centre_) : radius(radius_), alpha(alpha_), centre(centre_), texture_map(text_map), map_flag(true) {}

Vector3d SceneObject::calc_normal(const Point& surface_point) const {
    return (surface_point-centre).normalized();
}

cv::Vec3f SceneObject::colour(const Point& surface_point) const {
    if (!map_flag) {
        return solid_colour;
    }

    SphericalCoord sphere = cartesian_to_sphere(surface_point-centre);

    Pixel corresponding_pix = sphere_to_image_pixel(sphere, texture_map);

    return sample_pixel(corresponding_pix,texture_map);
}