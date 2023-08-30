#include "Camera.hpp"

namespace Vizior {

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