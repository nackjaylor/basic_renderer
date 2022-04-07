#pragma once

#include "CameraCal.h"
#include "CameraOps.h"
#include <vector>
#include "Ray.h"

class Camera {
    private:
        CameraCal _intrinsics;
        Point _position;
        Quat _rot;
        
        
    
    public:
        CameraOps ops;
        Camera(const CameraCal& intrinsics, const Point& position, const Quat& rot);
        
        std::vector<Ray> pixels_to_ray();

        inline void set_position(const Point& position) {_position = position;}
        inline void set_rotation(const Quat& rotation) {_rot = rotation;}
        inline Quat get_rotation() {return _rot;}
        inline Point get_position() {return _position;}
        inline HomogenousMatrix get_extrinsics() {return _extrinsics;}
        void move_camera(const Point& position, const Quat& rotation);
        void move_camera(const HomogenousMatrix& extrinsics);
        inline std::pair<int,int> image_dim() {return std::make_pair(_intrinsics.m_img_h,_intrinsics.m_img_w);}

    private:
        
        HomogenousMatrix _extrinsics = ops.extrinsic_matrix(_position,_rot);
        

};