#include "Renderer.h"
#include <algorithm>
#include <iostream>
Renderer::Renderer(Camera camera, model_name model_type) : camera(camera),_model_type(model_name(model_type)) {}

double Renderer::calculate_Lambert(const Point& intersect, const Light& light, const SceneObject& obj) {
    Vector3d light_dir = (light.get_location()-intersect).normalized();
    Vector3d sphere_Normal = obj.calc_normal(intersect);
    return std::max(0.0, light_dir.dot(sphere_Normal));
}

bool Renderer::check_shadow(const Point& intersect, const Light& light, const std::vector<SceneObject>& objs) {
    Ray vector_to_light = Ray(intersect,(light.get_location()-intersect).normalized());
    return std::get<0>(vector_to_light.intersection(objs));
}

void Renderer::set_ambient(const double& strength, const cv::Vec3f& colour) {
    set_ambient_strength(strength);
    set_ambient_colour(colour);
}

cv::Vec3f Renderer::colour_multiplied(const cv::Vec3f& surf_colour, const cv::Vec3f& illuminant) const {
    cv::Vec3f final_colour;
    cv::multiply(surf_colour,illuminant,final_colour);
    return final_colour;
}

cv::Mat Renderer::render() {
    
    cv::Mat image;
    render(image);

    return image;
}

void Renderer::render(cv::Mat& image) {

    std::pair<int,int> image_dims = camera.image_dim();
    
    image = cv::Mat::zeros(image_dims.first,image_dims.second,CV_32FC3);
    std::vector<Ray> rays = camera.pixels_to_ray();
    int pix_num = 0;
    for (int i = 0; i< image_dims.second; i++) {
        for (int j = 0; j < image_dims.first; j++) {
            
            Ray curr_ray = rays[pix_num];
            int recur_depth = 0;
            
            cv::Vec3f& colour = image.at<cv::Vec3f>(j,i);
            while (recur_depth < 10) {
                
                std::tuple<bool,SceneObject,double> intersect = curr_ray.intersection(objects);

                if (std::get<0>(intersect)) {
                    
                    Point point_of_intersect = curr_ray.sample(std::get<2>(intersect));
                    SceneObject obj_of_interest = std::get<1>(intersect);

                    for (Light light : lights) {
                        if (!check_shadow(point_of_intersect,light,objects)) {
                        
                        colour += (colour_multiplied(obj_of_interest.colour(point_of_intersect),light.colour)*(+calculate_Lambert(point_of_intersect,light,obj_of_interest))+blinn_phong(obj_of_interest,point_of_intersect,light,curr_ray)*light.colour)*curr_ray.intensity;
                        }

                    }

                    colour += ambient_strength*colour_multiplied(obj_of_interest.colour(point_of_intersect),ambient_colour)*curr_ray.intensity;
                    
                    
                

                    Vector3d normal = obj_of_interest.calc_normal(point_of_intersect);
                    double reflect = 2*curr_ray.get_direction().dot(normal);
                    Vector3d new_direction = curr_ray.get_direction()-reflect*normal;
                    double new_intensity = curr_ray.intensity*obj_of_interest.reflection_factor;
                    curr_ray = Ray(point_of_intersect,new_direction);
                    curr_ray.intensity = new_intensity;

                    recur_depth++;
                } else if (!std::get<0>(intersect) && recur_depth == 0) {
                    colour += background_colour;
                    
                    recur_depth = 10;
                } else {
                    recur_depth = 10;
                    
                    
                }
            }

            pix_num++;
        }
    }


}

void Renderer::render(cv::Mat& image, cv::Mat& depth) {
    std::pair<int,int> image_dims = camera.image_dim();
    
    image = cv::Mat::zeros(image_dims.first,image_dims.second,CV_32FC3);
    depth = cv::Mat::zeros(image_dims.first,image_dims.second,CV_32F);

    std::vector<Ray> rays = camera.pixels_to_ray();
    int pix_num = 0;

    for (int i = 0; i< image_dims.second; i++) {
        for (int j = 0; j < image_dims.first; j++) {
            
            Ray curr_ray = rays[pix_num];
            int recur_depth = 0;
            
            cv::Vec3f& colour = image.at<cv::Vec3f>(j,i);
            float& d_val = depth.at<float>(j,i);
            while (recur_depth < 10) {
                
                std::tuple<bool,SceneObject,double> intersect = curr_ray.intersection(objects);

                if (std::get<0>(intersect)) {
                    
                    Point point_of_intersect = curr_ray.sample(std::get<2>(intersect));
                    SceneObject obj_of_interest = std::get<1>(intersect);

                    for (Light light : lights) {
                        if (!check_shadow(point_of_intersect,light,objects)) {
                        
                        colour += (colour_multiplied(obj_of_interest.colour(point_of_intersect),light.colour)*(+calculate_Lambert(point_of_intersect,light,obj_of_interest))+blinn_phong(obj_of_interest,point_of_intersect,light,curr_ray)*light.colour)*curr_ray.intensity;
                        }

                    }

                    colour += ambient_strength*colour_multiplied(obj_of_interest.colour(point_of_intersect),ambient_colour)*curr_ray.intensity;
                    
                    if (recur_depth == 0) {
                        d_val = (point_of_intersect-curr_ray.get_origin()).norm();
                    }    
                

                    Vector3d normal = obj_of_interest.calc_normal(point_of_intersect);
                    double reflect = 2*curr_ray.get_direction().dot(normal);
                    Vector3d new_direction = curr_ray.get_direction()-reflect*normal;
                    double new_intensity = curr_ray.intensity*obj_of_interest.reflection_factor;
                    curr_ray = Ray(point_of_intersect,new_direction);
                    curr_ray.intensity = new_intensity;

                    


                    recur_depth++;
                } else if (!std::get<0>(intersect) && recur_depth == 0) {
                    colour += background_colour;
                    d_val = 0;
                    recur_depth = 10;

                } else {
                    recur_depth = 10;
                    
                    
                }
            }

            pix_num++;
        }
    }
}


cv::Mat Renderer::render_depth() {

    cv::Mat depth_image;
    render_depth(depth_image);
    return depth_image;
}

void Renderer::render_depth(cv::Mat& depth) {

    std::pair<int,int> image_dims = camera.image_dim();
    depth = cv::Mat::zeros(image_dims.first,image_dims.second,CV_32F);

    std::vector<Ray> rays = camera.pixels_to_ray();
    int pix_num = 0;

    for (int i = 0; i< image_dims.second; i++) {
        for (int j = 0; j < image_dims.first; j++) {
            
            Ray curr_ray = rays[pix_num];
            int recur_depth = 0;
            
            
            float& d_val = depth.at<float>(j,i);

                
                std::tuple<bool,SceneObject,double> intersect = curr_ray.intersection(objects);

                if (std::get<0>(intersect)) {
                    
                    Point point_of_intersect = curr_ray.sample(std::get<2>(intersect));

                    
                    d_val = (point_of_intersect-curr_ray.get_origin()).norm();
                     
    
                } else {
                    d_val = 0;
                }

            pix_num++;
        }
    }

}


double Renderer::blinn_phong(const SceneObject& object, const Point& intersection, const Light& light, const Ray& ray) {
    Vector3d normal = object.calc_normal(intersection);
    Vector3d light_direc = (light.get_location()-intersection).normalized();
    Vector3d view_direc = (intersection-ray.get_origin()).normalized();
    Vector3d blinn = (light_direc-view_direc).normalized();
    double blinn_val = std::max(blinn.dot(normal),0.0);
    return object.spec_value*pow(blinn_val,object.spec_power);
}