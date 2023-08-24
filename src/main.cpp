#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#include "Vizior.hpp"

std::shared_ptr<Vizior::Window> win1 = NULL, win2 = NULL;
std::shared_ptr<Vizior::ImageBuilder> src1 = NULL, src2 = NULL;
Vizior::Texture* containerImage;

void loop(int frameCount){
    std::cout << frameCount << std::endl; 
    Vizior::Point2D center = {400,300};
    Vizior::Point2D topr = {800,600};
    Vizior::Point2D topl = {0,600};
    Vizior::Point2D botr = {0,0};
    Vizior::Point2D botl = {800,0};

    Vizior::Point2D pt = {700,500};
    
    Vizior::Point2D t1[3] = {
        {0,0}, {0,599}, {799,0}
    };
    Vizior::Point2D t2[3] = {
        {800,600}, {800,1}, {1,600}
    };
    
    Vizior::Point2D l[4][2] = {
        {center, topr},
        {center, topl},
        {center, botr},
        {center, botl}
    };

    if(frameCount > 20){
        src1->drawTriangle(t1, Vizior::RED);
        src1->drawTriangle(t2, Vizior::BLU);
        for(int i = 0; i < 4; i++){
            src1->drawLine(l[i],(i+1)*4,Vizior::GRN);
        }
        src1->drawImage(Vizior::ANCHOR_TL, center, containerImage, 200,200,0);
        src1->drawCircle(center,55,Vizior::BLK);
        Vizior::Color PNK{255,9*16+9,14*16+14};
        src1->drawPoint(pt, 100, PNK);
        //src2->drawTriangle(t1, Vizior::BLU);
    }
    /*Vizior::drawCircle(Vizior::ANCHOR_C, &center, 100, &Vizior::BLU);
    Vizior::drawRectangle(Vizior::ANCHOR_C, &center, 700, 20, 45, &Vizior::GRN);
    
    Vizior::Point2D l1[2]{
        {600, 200}, {650,530}
    };

    Vizior::drawLine(l1, 2, &Vizior::BLK);

    Vizior::Point2D l2[2]{
        {0,0},{(frameCount/2)%800,(frameCount/2)%600}
    };
    Vizior::drawLine(l2, 10, &PNK);


    Vizior::drawAll();*/
}

int main(void)
{
    const char *winName1 = "First Window";
    const char *winName2 = "Second Window";
    const char *image = "../container.jpg";

    win1 = std::make_shared<Vizior::Window>(800,600,winName1);
    src1 = std::make_shared<Vizior::ImageBuilder>();
    win1->setSource(src1);
    containerImage = new Vizior::Texture(image);
    std::cout << "Register window with id " << Vizior::registerWindow(win1) << std::endl;

    /*win2 = std::make_shared<Vizior::Window>(800,600,winName2);
    src2 = std::make_shared<Vizior::ImageBuilder>();
    win2->setSource(src2);
    std::cout << "Register window with id " << Vizior::registerWindow(win2) << std::endl;*/

    Vizior::setLoopFunc(loop);
    //std::cin.get();
    Vizior::Start();
    
    return 0;
}