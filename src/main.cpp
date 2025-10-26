#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#include "Texture.hpp"
#include "Vizior.hpp"
#include "Window.hpp"
#include "Common.hpp"

std::shared_ptr<vzr::Window> win1 = NULL, win2 = NULL;
std::shared_ptr<vzr::ImageBuilder> src1 = NULL, src2 = NULL;
vzr::Texture* containerImage;

const char *winName1 = "First Window";
const char *winName2 = "Second Window";
const char *image = "../container.jpg";
const char *fontPath = "../abys.ttf";
const char *fontName = "abys";

vzr::FontManager *fontManager = vzr::FontManager::getInstance();

bool lastFrameMouseClicked = false;
int newCircleRadius = 0;
vzr::Point2D newCirclePos;


void loop(int frameCount, double timestep){
    std::cout << frameCount << std::endl;
    

    vzr::Point2D center = {400,300};
    vzr::Point2D other = {200,150};
    vzr::Point2D topr = {800,600};
    vzr::Point2D topl = {0,600};
    vzr::Point2D botr = {800,0};
    vzr::Point2D botl = {0,0};

    vzr::Point2D pt = {700,500};
    
    vzr::Point2D t1[3] = {
        {0,0}, {0,599}, {799,0}
    };
    vzr::Point2D t2[3] = {
        {800,600}, {800,1}, {1,600}
    };
    
    vzr::Point2D l[4][2] = {
        {center, topr},
        {center, topl},
        {center, botr},
        {center, botl}
    };

    vzr::Color CYN {0,100,100,255};
    vzr::Point2D poly[5] = {
        {50,50},
        {100,0},
        {50,-50},
        {-50,-50},
        {-50,50}
    };

    vzr::Color prl = {127,0,255,255};
    vzr::Point2D spiral[4*5];
    for (int i = 0; i < 5; i++){
        spiral[i*4+0]= {10+i*40,10+i*30};
        spiral[i*4+1]= {800-(10+i*40),10+i*30};
        spiral[i*4+2]= {800-(10+i*40),600-(10+i*30)};
        spiral[i*4+3]= {10+i*40,600-(10+i*30)};
    }

    vzr::Point2D bezier[4] = {
        botl,
        topr,
        botr,
        topl,
    };



    src1->drawTriangle(t1, vzr::RED);
    src1->drawTriangle(t2, vzr::BLU);
    src1->drawImage(vzr::ANCHOR_TL, center, containerImage, 200,200,0);
    for(int i = 0; i < 4; i++){
        src1->drawLine(l[i],(i+1)*4,vzr::GRN);
    }
    vzr::Color PNK{255,9*16+9,14*16+14,255};
    src1->drawCircle(center,10, PNK);
    src1->drawPoint(pt, 100, PNK);
    src1->drawText(vzr::ANCHOR_BL,other,"The quick brown fox", vzr::WHT, fontName, 1, 0);
    src1->drawLine(spiral, 20, 5, prl, false);
    src1->drawPolygon(poly, 5, CYN);
    src1->drawArc(center, 100,33,133,vzr::ORG);
    src1->drawRing(center, 135,155, vzr::CYN);
    std::cout << "Got here" << std::endl;
    src1->drawRingArc(center, 155,175, 45,225, vzr::DRK_RED);
    src1->drawEllipse(topr, 150,100,135,vzr::SKY_BLU);
    //src2->drawTriangle(t1, vzr::BLU);
    src1->drawBezier(bezier[0], bezier[1], bezier[2], bezier[3], vzr::PRPL);

    if(win1->isMousePressed()){
        newCircleRadius++;
        lastFrameMouseClicked = true;
        newCirclePos = win1->getMouseWorldPos();
    } else if(lastFrameMouseClicked == true) {
        newCircleRadius = 0;
        lastFrameMouseClicked = false;
    }

    src1->drawCircle(newCirclePos, newCircleRadius, vzr::DRK_GRN);

    if(win1->isKeyPressed(vzr::Key::KEY_Y)){
        src1->drawCircle(botl, 100, vzr::DRK_RED);
    }

    vzr::Color grid[9] = {vzr::BLU, vzr::BLU, vzr::BLU, vzr::BLU, vzr::RED, vzr::BLU, vzr::BLU, vzr::BLU, vzr::BLU};
    src1->drawGrid(botl, 500, 500, grid, 3, 3);


    vzr::Color myCols[100*100];
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100; j++){
            myCols[i*100 + j] = {static_cast<uint8_t>(255*i/100), 0, static_cast<uint8_t>(255*j/100), 255};
        }
    }
    vzr::Texture* myTex = vzr::Texture::fromPixels(100, 100, myCols) ;
    src1->drawImage(vzr::ANCHOR_TL, center, myTex, 100, 100, 0);
    delete myTex;
}

int main(void)
{

    win1 = std::make_shared<vzr::Window>(800,600,winName1);
    src1 = std::make_shared<vzr::ImageBuilder>();
    win1->setSource(src1);
    containerImage = vzr::Texture::fromImage(image);
    std::cout << "Register window with id " << vzr::registerWindow(win1) << std::endl;
    std::cout << "Created texture from image " << containerImage << " " << containerImage->getID() << std::endl;
    fontManager->registerFont(fontPath, fontName);
    std::cout << "Registered font " << fontName << std::endl;
    vzr::Color BRN{102,51,0,255};
    //src1->setBackgroundColor(BRN);
    
    //win2 = std::make_shared<vzr::Window>(800,600,winName2);
    //win2->setSource(src1);
    //std::cout << "Register window with id " << vzr::registerWindow(win2) << std::endl;
    /*src2 = std::make_shared<vzr::ImageBuilder>();
    win2->setSource(src2);
    std::cout << "Register window with id " << vzr::registerWindow(win2) << std::endl;*/
    
    vzr::setLoopFunc(loop);
    //std::cin.get();
    vzr::Start();
    
    return 0;
}