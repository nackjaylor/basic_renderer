#include "Camera.h"

#include <iostream>

Camera::Camera(const CameraCal& intrinsics, const Point& position, const Quat& rot) : _intrinsics(intrinsics), _position(position), _rot(rot), ops(_intrinsics) {}


void Camera::move_camera(const Point& position, const Quat& rotation) {
    set_position(position);
    set_rotation(rotation);
    _extrinsics = ops.extrinsic_matrix(_position,_rot);
}

void Camera::move_camera(const HomogenousMatrix& extrinsics) {
    Point new_pos;
    Quat new_rot;
    ops.extrinsics_to_singular(extrinsics,new_pos,new_rot);
    move_camera(new_pos,new_rot);
}

std::vector<Ray> Camera::pixels_to_ray() {

    PixelArray pixel_locs;
    std::vector<Ray> output;
    
    pixel_locs.resize(NoChange,_intrinsics.number_pixels());

    int counter = 0;

    

    for (int i = 0; i<_intrinsics.m_img_w; i++) {

        for (int j = 0; j<_intrinsics.m_img_h; j++) {

            pixel_locs(0,counter) = i+0.5;
            pixel_locs(1,counter) = j+0.5;
            
            counter++;
            
        
        }
    }

    // std::cout << "Pixel_Locs: " << pixel_locs << "\n";
    PointArray world_coords = ops.pixel_to_world(pixel_locs,_extrinsics, 1);
    world_coords = world_coords.colwise()-_position;
    world_coords.colwise().normalize();
    // std::cout << "World_Coords: " << world_coords << "\n";

    // std::cout << "Out: " << world_coords << "\n";

    
    for (int i =0; i<world_coords.cols(); i++) {
        output.emplace_back(Ray(_position,world_coords(all,i)));
    }

    return output;


}