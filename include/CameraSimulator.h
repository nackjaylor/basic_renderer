#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "types.h"
#include <vector>
#include "SimImage.h"

class CameraSimulator {


    public:

        Camera camera;
        Renderer renderer;

        CameraSimulator(const Camera& camera_, const Renderer& renderer_);

        Point generate_position_from_altitude(const double& altitude, const double& altitude_variance = 5, const double& phi_datum = 0);

        Point generate_position_from_spherical(const double& rho, const double& theta, const double& phi);

        Point generate_position_from_radius(const double& rho, const double& rho_variance = 5, const double& phi_datum = 0);

        Point generate_position_from_radius_2D(const double& rho, const double& rho_variance = 5);

        SimImage random_observation_altitude(const double& altitude);
        SimImage random_observation_radius(const double& radius);

        PixelArray simulate_image_pixels(const PointArray& data, const Point& camera, const Quat& pose);
        PixelArray simulate_image_pixels(const PointArray& data, const HomogenousMatrix& extrinsics);

        std::vector<SimImage> n_random_observations(const int& n, const double& radius);


};