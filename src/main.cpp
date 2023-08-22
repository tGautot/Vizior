#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#include "Vizior.hpp"

std::shared_ptr<Vizior::Window> win = NULL;
std::shared_ptr<Vizior::ImageBuilder> src = NULL;

void loop(int frameCount){
    std::cout << frameCount << std::endl; 
    Vizior::Point2D t1[3] = {
        {0,0}, {0,599}, {799,0}
    };
    Vizior::Point2D center = {400,300};
    if(frameCount > 20)
        src->drawTriangle(t1, Vizior::RED);
    /*Vizior::drawCircle(Vizior::ANCHOR_C, &center, 100, &Vizior::BLU);
    Vizior::drawRectangle(Vizior::ANCHOR_C, &center, 700, 20, 45, &Vizior::GRN);
    
    Vizior::Point2D l1[2]{
        {600, 200}, {650,530}
    };

    Vizior::drawLine(l1, 2, &Vizior::BLK);

    Vizior::Point2D l2[2]{
        {0,0},{(frameCount/2)%800,(frameCount/2)%600}
    };
    Vizior::Color PNK{255,9*16+9,14*16+14};
    Vizior::drawLine(l2, 10, &PNK);


    Vizior::drawAll();*/
}

int main(void)
{
    const char *winName = "Triangles??";
    std::cout << winName << std::endl; 

    win = std::make_shared<Vizior::Window>(800,600,winName);
    src = std::make_shared<Vizior::ImageBuilder>();
    win->setSource(src);
    Vizior::registerWindow(win);
    Vizior::setLoopFunc(loop);
    Vizior::Start();
    
    return 0;
}