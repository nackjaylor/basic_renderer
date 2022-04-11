#include "CameraSimulator.h"

#include "CameraOps.h"
#include <cmath>
#include "random_float.h"
#include "types.h"
#include <iostream>

CameraSimulator::CameraSimulator(const Camera& camera_, const Renderer& renderer_)
    : camera(camera_), renderer(renderer_) {}

Point CameraSimulator::generate_position_from_altitude(const double& altitude, const double& altitude_variance, const double& phi_datum) {
    double z = altitude+(2*random_float()-1)*altitude_variance;
    double phi_loc = phi_datum+M_PI_4*random_float();
    double theta_loc = 2*M_PI*random_float();

    double rho = z/cos(phi_loc);
    // std::cout << "Phi_loc:" << phi_loc << "\n";
    double x = rho*cos(theta_loc)*sin(phi_loc);
    double y = rho*sin(theta_loc)*sin(phi_loc);
    // std::cout << "x" << x << "\n";
    Point out;
    out << x,y,z;

    return out;

}

Point CameraSimulator::generate_position_from_spherical(const double& rho, const double& theta, const double& phi) {
    double x = rho*cos(theta)*sin(phi);
    double y = rho*sin(theta)*sin(phi);
    double z = rho*cos(phi);

    Point out;
    out << x,y,z;

    return out;
}

Point CameraSimulator::generate_position_from_radius(const double& rho, const double& rho_variance, const double& phi_datum) {
    double phi_loc = phi_datum+M_PI_4*random_float();
    double theta_loc = 2*M_PI*random_float();

    double rho_actual = rho+(2*random_float()-1)*rho_variance;

    return generate_position_from_spherical(rho_actual, theta_loc, phi_loc);
}

Point CameraSimulator::generate_position_from_radius_2D(const double& rho, const double& rho_variance) {
    double theta_loc = 2*M_PI*random_float();

    double rho_actual = rho+(2*random_float()-1)*rho_variance;

    return generate_position_from_spherical(rho_actual, theta_loc, M_PI_2);
}

SimImage CameraSimulator::random_observation_altitude(const double& altitude) {

    Point camera_location = generate_position_from_altitude(altitude);

    // std::cout << "Camera Location: " << camera_location << "\n";
    

    HomogenousMatrix extrinsics = camera.ops.look_at(camera_location);
    Point new_pos;
    Quat rotation;
    camera.ops.extrinsics_to_singular(extrinsics,new_pos,rotation);
    // std::cout << "Extrinsics: " << extrinsics << "\n";

    renderer.camera.move_camera(camera_location,rotation);
    
    SimImage sim_image;
    sim_image.image = renderer.render();
    sim_image.position = renderer.camera.get_position();
    sim_image.pose = renderer.camera.get_rotation();
    
    
    return sim_image;
}

SimImage CameraSimulator::random_observation_radius(const double& radius) {
    Point camera_location = generate_position_from_radius_2D(radius);

    // std::cout << "Camera Location: " << camera_location << "\n";
    

    HomogenousMatrix extrinsics = camera.ops.look_at(camera_location);
    Point new_pos;
    Quat rotation;
    camera.ops.extrinsics_to_singular(extrinsics,new_pos,rotation);


    renderer.camera.move_camera(camera_location,rotation);

    
    SimImage sim_image;
    renderer.render(sim_image.image,sim_image.depth);
    sim_image.position = renderer.camera.get_position();
    sim_image.pose = renderer.camera.get_rotation();
    
    
    return sim_image;
}

PixelArray CameraSimulator::simulate_image_pixels(const PointArray& data, const HomogenousMatrix& extrinsics) {
    
    PixelArray new_pix = camera.ops.world_to_pixel(data,extrinsics);

    new_pix = new_pix.array().rint();

    std::vector<Vector2d> vec;

    for  (int i = 0; i < new_pix.cols(); i++) {
        vec.push_back(new_pix.col(i));
    }

    auto it = std::unique(vec.begin(),vec.end());
    vec.resize(std::distance(vec.begin(),it));

    PixelArray binned_pix;
    binned_pix.resize(NoChange,vec.size());

    for (int i = 0; i<vec.size();i++) {
        binned_pix.col(i) = vec[i];
    }

    return binned_pix;
}

std::vector<SimImage> CameraSimulator::n_random_observations(const int& n, const double& radius) {
    std::vector<SimImage> out;

    for (int i =0; i<n; i++) {
        out.push_back(random_observation_radius(radius));
    }

    return out;
}