#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "vizior.hpp"

int main(void)
{
    const char *winName = "Triangles??";
    vizior::createWindow(800,600,winName);
    vizior::init();
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
    vizior::drawAll();
    return 0;
}