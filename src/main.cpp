#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "vizior.hpp"

void loop(int frameCount){
    vizior::Point2D t1[3] = {
        {0,0}, {0,599}, {799,0}
    };
    vizior::Point2D center = {400,300};
    vizior::drawTriangle(t1, &vizior::RED);
    vizior::drawCircle(vizior::ANCHOR_C, &center, 100, &vizior::BLU);
    vizior::drawRectangle(vizior::ANCHOR_C, &center, 700, 20, 45, &vizior::GRN);
    
    vizior::Point2D l1[2]{
        {600, 200}, {650,530}
    };

    vizior::drawLine(l1, 2, &vizior::BLK);

    vizior::Point2D l2[2]{
        {0,0},{(frameCount/2)%800,(frameCount/2)%600}
    };
    vizior::Color PNK{255,9*16+9,14*16+14};
    vizior::drawLine(l2, 10, &PNK);


    vizior::drawAll();
}

int main(void)
{
    const char *winName = "Triangles??";
    vizior::createWindow(800,600,winName);
    vizior::init();
    vizior::setLoopFunc(loop);
    vizior::Start();
    
    return 0;
}