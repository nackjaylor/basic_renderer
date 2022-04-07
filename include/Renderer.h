#pragma once

#include "types.h"
#include <string>
#include "Camera.h"
#include "Light.h"
#include "SceneObject.h"
#include "Ray.h"
#include <opencv2/core.hpp>

enum model_name {BLINN_PHONG, PHONG};

class Renderer {
    public:

        Camera camera;
        Renderer(Camera camera, model_name model_type = BLINN_PHONG);

        inline int get_model() {return _model_type;}

        static double calculate_Lambert(const Point& intersect, const Light& light, const SceneObject& obj);
        static bool check_shadow(const Point& intersect, const Light& light, const std::vector<SceneObject>& obj);

        cv::Mat render();

        inline void add_light(const Light& light) {lights.emplace_back(light);}

        inline void add_object(const SceneObject& obj) {objects.emplace_back(obj);}
        
        inline void set_ambient_strength(const double& strength) {ambient_strength = strength;}
        inline void set_ambient_colour(const cv::Vec3f& colour) {ambient_colour = colour;}

        void set_ambient(const double& strength, const cv::Vec3f& colour);

        inline void set_background_colour(const cv::Vec3f& colour) {background_colour = colour;}

        cv::Vec3f colour_multiplied(const cv::Vec3f& surf_colour, const cv::Vec3f& illuminant) const;

        double blinn_phong(const SceneObject& object, const Point& intersection, const Light& light, const Ray& ray);




    private:

        std::vector<Light> lights;
        std::vector<SceneObject> objects;
        double ambient_strength = 0.1;
        cv::Vec3f ambient_colour = cv::Vec3f(1.0,1.0,1.0);
        cv::Vec3f background_colour = cv::Vec3f(0.1,0.0,0.0);

        int _model_type;
        
};