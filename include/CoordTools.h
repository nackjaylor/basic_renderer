#pragma once
#include "types.h"
#include <opencv2/core.hpp>
struct SphericalCoord {
    double rho, theta, phi;
};


SphericalCoord cartesian_to_sphere(const Point& cartesian) {
    SphericalCoord spherical;

    spherical.rho = cartesian.norm();
    spherical.theta = acos(cartesian(2)/spherical.rho);
    spherical.phi = atan2(cartesian(1),cartesian(0));


    return spherical;
}

Point sphere_to_cartesian(const SphericalCoord& spherical) {
    Point cart;
    cart(0) = spherical.rho*sin(spherical.phi)*cos(spherical.theta);
    cart(1) = spherical.rho*sin(spherical.phi)*sin(spherical.theta);
    cart(2) = spherical.rho*cos(spherical.phi);
    return cart;
}


Pixel sphere_to_image_pixel(const SphericalCoord& spherical, const cv::Mat& image) {

    // theta: 0 -> 2*pi, phi: 0 -> pi
    Pixel index;
    index(0) = (spherical.phi/(2*M_PI))*image.cols;
    index(1) = (spherical.theta/M_PI)*image.rows;

    // if (spherical.phi<M_PI) {
    //     index(0) = (spherical.theta/M_PI)*image.cols;
    //     index(1) = (spherical.phi/M_PI)*image.rows;
    // } else {
    //     index(0) = (spherical.theta/M_PI+0.5)*image.cols;
    //     index(1) = (1-(spherical.phi-2*M_PI)/(M_PI))*image.rows;
    // }
    
    return index;

}

cv::Vec3f sample_pixel(const Pixel& pix_location, const cv::Mat& image) {
    Pixel min_val = pix_location.array().floor();
    Pixel max_val = pix_location.array().ceil();

    cv::Vec3f min_colour = image.at<cv::Vec3f>(int(min_val(1)),int(min_val(0)));
    cv::Vec3f max_colour = image.at<cv::Vec3f>(int(max_val(1)),int(max_val(0)));
    double weighting = ((pix_location-min_val)).norm()/(max_val-min_val).norm();
    return weighting*min_colour + (1-weighting)*max_colour;
}