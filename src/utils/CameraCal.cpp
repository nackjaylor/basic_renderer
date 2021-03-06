/*
Part of Camera Projections using Eigen - a set of classes for computer vision transformations.
Copyright (C) 2021  Jack Naylor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. 
*/


#include "CameraCal.h"
#include <iostream>
#include "Eigen/Core"




CameraCal::CameraCal(double fx, int img_w)  
    :   m_fx(fx), 
        m_img_w(img_w) {
}

CameraCal::CameraCal(double fx, double fy, int img_w, int img_h)
    :   m_fx(fx),
        m_fy(fy),
        m_img_w(img_w), 
        m_img_h(img_h),
        m_camera_matrix(Eigen::Matrix3d::Identity()) {

}

CameraCal::CameraCal(double fx, double fy, int img_w, int img_h, double ppx, double ppy, double k1, double k2, double k3, double p1, double p2)
    :   m_fx(fx),
        m_fy(fy),
        m_img_w(img_w), 
        m_img_h(img_h),
        m_ppx(ppx),
        m_ppy(ppy),
        m_k1(k1),
        m_k2(k2),
        m_k3(k3),
        m_p1(p1),
        m_p2(p2),
        m_camera_matrix(Eigen::Matrix3d::Identity()) {

}


CameraCal::CameraCal(double fx, double fy, int img_w, int img_h, double ppx, double ppy, double k1, double k2, double k3, double p1, double p2, double b1, double b2)
    :   m_fx(fx),
        m_fy(fy),
        m_img_w(img_w), 
        m_img_h(img_h),
        m_ppx(ppx),
        m_ppy(ppy),
        m_k1(k1),
        m_k2(k2),
        m_k3(k3),
        m_p1(p1),
        m_p2(p2),
        m_b1(b1),
        m_b2(b2),
        m_camera_matrix(Eigen::Matrix3d::Identity()) {

}

double CameraCal::getPPx() {
    return m_ppx;
}

double CameraCal::getFy() {
    return m_fy;
}

void CameraCal::constructMatrix() {
    m_camera_matrix(0,0) = m_fx/m_pix_size;
    m_camera_matrix(1,1) = m_fy/m_pix_size;
    m_camera_matrix(0,2) = m_ppx;
    m_camera_matrix(1,2) = m_ppy;
    
}

Eigen::Matrix3d CameraCal::new_matrix() {
    Eigen::Matrix3d cal_matrix = Eigen::Matrix3d::Identity();
    cal_matrix(0,0) = m_fx/m_pix_size+m_b1;
    cal_matrix(0,1) = m_b2;
    cal_matrix(1,1) = m_fy/m_pix_size;
    cal_matrix(0,2) = m_ppx;
    cal_matrix(1,2) = m_ppy;

    return cal_matrix;

}

Eigen::Matrix3d CameraCal::getMatrix() const {

    return m_camera_matrix;
}

bool operator== (const CameraCal& lhs, const CameraCal& rhs) {
    if (lhs.m_camera_matrix == rhs.m_camera_matrix) {
        return true;
    } else {
        return false;
    }
}

