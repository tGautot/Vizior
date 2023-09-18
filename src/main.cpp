#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#include "Vizior.hpp"

std::shared_ptr<Vizior::Window> win1 = NULL, win2 = NULL;
std::shared_ptr<Vizior::ImageBuilder> src1 = NULL, src2 = NULL;
Vizior::Texture* containerImage;

const char *winName1 = "First Window";
const char *winName2 = "Second Window";
const char *image = "../container.jpg";
const char *fontPath = "../abys.ttf";
const char *fontName = "abys";

Vizior::FontManager *fontManager = Vizior::FontManager::getInstance();

bool lastFrameMouseClicked = false;
int newCircleRadius = 0;
Vizior::Point2D newCirclePos;


void loop(int frameCount){
    std::cout << frameCount << std::endl;
    

    Vizior::Point2D center = {400,300};
    Vizior::Point2D other = {200,150};
    Vizior::Point2D topr = {800,600};
    Vizior::Point2D topl = {0,600};
    Vizior::Point2D botr = {800,0};
    Vizior::Point2D botl = {0,0};

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

    Vizior::Color CYN {0,100,100,255};
    Vizior::Point2D poly[5] = {
        {50,50},
        {100,0},
        {50,-50},
        {-50,-50},
        {-50,50}
    };

    Vizior::Color prl = {127,0,255,255};
    Vizior::Point2D spiral[4*5];
    for (int i = 0; i < 5; i++){
        spiral[i*4+0]= {10+i*40,10+i*30};
        spiral[i*4+1]= {800-(10+i*40),10+i*30};
        spiral[i*4+2]= {800-(10+i*40),600-(10+i*30)};
        spiral[i*4+3]= {10+i*40,600-(10+i*30)};
    }

    Vizior::Point2D bezier[4] = {
        botl,
        topr,
        botr,
        topl,
    };



    src1->drawTriangle(t1, Vizior::RED);
    src1->drawTriangle(t2, Vizior::BLU);
    src1->drawImage(Vizior::ANCHOR_TL, center, containerImage, 200,200,0);
    for(int i = 0; i < 4; i++){
        src1->drawLine(l[i],(i+1)*4,Vizior::GRN);
    }
    Vizior::Color PNK{255,9*16+9,14*16+14,255};
    src1->drawCircle(center,10, PNK);
    src1->drawPoint(pt, 100, PNK);
    src1->drawText(Vizior::ANCHOR_BL,other,"The quick brown fox", Vizior::WHT, fontName, 1, 0);
    src1->drawLine(spiral, 20, 5, prl, false);
    src1->drawPolygon(poly, 5, CYN);
    src1->drawArc(center, 100,33,133,Vizior::ORG);
    src1->drawRing(center, 135,155, Vizior::CYN);
    std::cout << "Got here" << std::endl;
    src1->drawRingArc(center, 155,175, 45,225, Vizior::DRK_RED);
    src1->drawEllipse(topr, 150,100,135,Vizior::SKY_BLU);
    //src2->drawTriangle(t1, Vizior::BLU);
    src1->drawBezier(bezier[0], bezier[1], bezier[2], bezier[3], Vizior::PRPL);

    if(win1->isMousePressed()){
        newCircleRadius++;
        lastFrameMouseClicked = true;
        newCirclePos = win1->getMouseWorldPos();
    } else if(lastFrameMouseClicked == true) {
        newCircleRadius = 0;
        lastFrameMouseClicked = false;
    }

    src1->drawCircle(newCirclePos, newCircleRadius, Vizior::DRK_GRN);

    if(win1->isKeyPressed(GLFW_KEY_Y)){
        src1->drawCircle(botl, 100, Vizior::DRK_RED);
    }

}

int main(void)
{

    win1 = std::make_shared<Vizior::Window>(800,600,winName1);
    src1 = std::make_shared<Vizior::ImageBuilder>();
    win1->setSource(src1);
    std::cout << "Window " << win1->getHeight() << " linked to src" << std::endl;
    containerImage = Vizior::Texture::fromImage(image);
    std::cout << "Register window with id " << Vizior::registerWindow(win1) << std::endl;
    std::cout << "Created texture from image " << containerImage << " " << containerImage->getID() << std::endl;
    fontManager->registerFont(fontPath, fontName);
    std::cout << "Registered font " << fontName << std::endl;
    Vizior::Color BRN{102,51,0,255};
    //src1->setBackgroundColor(BRN);
    
    //win2 = std::make_shared<Vizior::Window>(800,600,winName2);
    //win2->setSource(src1);
    //std::cout << "Register window with id " << Vizior::registerWindow(win2) << std::endl;
    /*src2 = std::make_shared<Vizior::ImageBuilder>();
    win2->setSource(src2);
    std::cout << "Register window with id " << Vizior::registerWindow(win2) << std::endl;*/
    
    Vizior::setLoopFunc(loop);
    //std::cin.get();
    Vizior::Start();
    
    return 0;
}