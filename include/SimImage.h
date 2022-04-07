#pragma once

#include "types.h"
#include <opencv2/core.hpp>
struct SimImage
{
    
    cv::Mat image;
    Point position;
    Quat pose;
};
