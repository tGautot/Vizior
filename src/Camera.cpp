#include "Camera.hpp"

namespace Vizior {

void Camera::setZoom(float z){
    if(z < 0.0001f){
        std::cout << "WARNING::CAMERA::SET_ZOOM: tried setting zoom value below minimum treshold, clipping value" << std::endl;
        m_Zoom = 0.0001f;
        return;
    }
    m_Zoom = z;
}

LinearCamera::LinearCamera(Point2D sttPos) : Camera(sttPos){
    m_Speed = 3;
}

void LinearCamera::movePos(Point2D dir){
    std::cout << "Moving camera by " << dir.x << "," << dir.y << std::endl;
    if(dir.len() < 0.01f){return;}
    dir /= dir.len();
    m_Pos += dir*m_Speed;
}


}