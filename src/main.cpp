#include <iostream>
#include "CameraCal.h"
#include "CameraOps.h"
#include "Renderer.h"
#include "Camera.h"
#include "Ray.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "CameraSimulator.h"
#include "SimImage.h"
#include <string>
#include <fstream>
#include <stdlib.h>  

std::string quaternion_to_string(const Quat& quaternion) {


    std::string result;
    result.reserve(50);

    result += std::to_string(quaternion.x());
	result += ",";
	result += std::to_string(quaternion.y());
	result += ",";
	result += std::to_string(quaternion.z());
	result += ",";
    result += std::to_string(quaternion.w());

    return result;
}

int main(int argc, char** argv) {

    srand(time(NULL));

    CameraCal cc(15e-3,15e-3,1920,1080);
    cc.constructMatrix();    
    CameraOps ops(cc);
    
    Point init_cam_loc(0,-15,0);
    Point cam_loc;
    HomogenousMatrix extrinsics = ops.look_at(init_cam_loc);
    
    Quat rotation;
    ops.extrinsics_to_singular(extrinsics,cam_loc,rotation);
    
    Camera cam(cc,init_cam_loc,rotation);
    
    cv::Mat moon_map = cv::imread("./texture_maps/william_osman_head.jpg");
    cv::Mat mars_map = cv::imread("./texture_maps/8k_mars.jpg");
    moon_map.convertTo(moon_map,CV_32FC3);
    mars_map.convertTo(mars_map,CV_32FC3);

    Renderer rendy(cam,BLINN_PHONG);

    Point light_loc(4,-8,0);
    Light light1(light_loc,light_loc,cv::Vec3f(1.0,1.0,1.0));
    rendy.add_light(light1);
    // Point light_loc2(-1,-2,0);
    // Light light2(light_loc2,light_loc2,cv::Vec3f(0,0.3,0.1));
    // rendy.add_light(light2);
    Point sphere_loc(3.5,1.5,0);
    SceneObject sphere1(0.8,1,moon_map/255,sphere_loc);
    sphere1.reflection_factor = 0.05;
    sphere1.spec_value = 0.1;
    rendy.add_object(sphere1);
    Point sphere_loc_2(-3,-2.5,0);
    SceneObject sphere2(4,1,mars_map/255,sphere_loc_2);
    sphere2.reflection_factor = 0.01;
    sphere2.spec_value = 0.1;
    rendy.add_object(sphere2);
    rendy.set_ambient_strength(0.2);
    std::cout << rendy.get_model() << "\n";

    
    

    int counter = 0;

    CameraSimulator sim(cam,rendy);

    std::vector<SimImage> output = sim.n_random_observations(10,40);
    IOFormat CommaInitFmt(FullPrecision, DontAlignCols, ",", ",", "", "", "", "");
    for (SimImage im : output) {
        cv::imwrite("./dataset6/image"+std::to_string(counter)+".png",255*im.image);
        cv::imwrite("./dataset6/depth_raw"+std::to_string(counter)+".tiff",im.depth);
        std::ofstream camera_info("./dataset6/extrins"+std::to_string(counter)+".csv");
        camera_info << im.position.format(CommaInitFmt) << "\n";
        
        camera_info << quaternion_to_string(im.pose);

        camera_info.close();

        counter++;
    }



    return 0;
}