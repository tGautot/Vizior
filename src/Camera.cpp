#include "Camera.hpp"

namespace vzr {

void Camera::setZoom(float z){
    if(z < 0.0001f){
        std::cout << "WARNING::CAMERA::SET_ZOOM: tried setting zoom value below minimum treshold, clipping value" << std::endl;
        m_Zoom = 0.0001f;
        return;
    }
    m_Zoom = z;
}

void Camera::computeLookAtMatrix(float* dest){
    // Look at matrix is computed as follows (matrix mult below)
    //
    // Rx Ry Rz 0       1  0  0 -Px
    // Ux Uy Uz 0 TIMES 0  1  0 -Py
    // Dx Dy Dz 0       0  0  1 -Pz
    // 0  0  0  1       0  0  0  1
    //
    // Since in our case, the camera will only move on a plane (z=cst)
    // We can make some simplification: Dx = 0, Dy = 0, Dz = -1 (assuming positive z in screen)
    // Also Rz = Uz = 0 and we can set Pz to 0 as well as this means we wont change z for vertex
    // This gives us the following 
    //
    // Rx Ry 0  0       1  0  0 -Px
    // Ux Uy 0  0 TIMES 0  1  0 -Py
    // 0  0  -1 0       0  0  1  0
    // 0  0  0  1       0  0  0  1
    //
    // Answer
    //
    // Rx Ry 0 -(Px*Rx)-(Py*Ry)
    // Ux Uy 0 -(Px*Ux)-(Py*Uy)
    // 0  0 -1  0
    // 0  0  0  1
    //
    // Its bare-bone, no glm needed
    
    float rx = cos(m_RotZ), ry = sin(m_RotZ);
    float ux = cos(m_RotZ+M_PI_2), uy = sin(m_RotZ+M_PI_2);
    float px = m_Pos.x, py = m_Pos.y;
    // since cos and sin, already normalized
    

    dest[0] = rx; dest[1] = ry; dest[2] = 0; dest[3] = -(px*rx + py*ry);
    dest[4] = ux; dest[5] = uy; dest[6] = 0; dest[7] = -(px*ux + py*uy);
    dest[8] = 0; dest[9] = 0; dest[10] = -1; dest[11] = 0;
    dest[12] = 0; dest[13] = 0; dest[14] = 0; dest[15] = 1;
    

}

LinearCamera::LinearCamera(Point2D sttPos) : Camera(sttPos){
    m_Speed = 3;
}

void LinearCamera::movePos(Point2D dir){
    //std::cout << "Moving camera by " << dir.x << "," << dir.y << std::endl;
    if(dir.len() < 0.01f){return;}
    dir /= dir.len();
    dir = dir.rotateAroundBy({0,0},180*m_RotZ/M_PI);
    m_Pos += dir*m_Speed;
}


}