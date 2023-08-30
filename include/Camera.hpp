#ifndef VIZIOR_CAMERA_H
#define VIZIOR_CAMERA_H

#include <Point2D.hpp>
#include <iostream>

namespace Vizior {

class Camera {
public:
    Camera(Point2D startPos){
        m_Pos = {startPos.x, startPos.y};
        m_Zoom = 1.0f;
    }


    // Move the camera in the given direction
    // Magnitude of the point has no effect
    virtual void movePos(Point2D dir) = 0;

    void setPos(Point2D pos){m_Pos = pos;}
    Point2D getPos(){return m_Pos;}
    void setZoom(float z);
    float getZoom(){return m_Zoom;}
protected:
    Point2D m_Pos;
    float m_Zoom;

};

class LinearCamera : public Camera {
public:
    LinearCamera(Point2D startPos);

    void movePos(Point2D dir);
    void setSpeed(int s){m_Speed = s;}
    int getSpeed(){return m_Speed;}
protected:
    int m_Speed;

};

}

#endif