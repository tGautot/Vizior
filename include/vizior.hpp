#ifndef VIZIOR_H
#define VIZIOR_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

namespace vizior {

    enum ANCHOR {
        ANCHOR_TR, ANCHOR_TL, ANCHOR_BR, ANCHOR_BL, ANCHOR_C // Top-Bottom Left-Right Center
    };

    typedef struct {
        int x; int y;
    } Point2D;

    typedef struct {
        uint8_t r, g, b, a;
    } Color;

    extern Color RED;
    extern Color GRN;
    extern Color BLU;
    extern Color WHT;
    extern Color BLK;

    extern int16_t targetFrameRate;
    extern int32_t winW, winH;

    // Pipeline relative

    int createWindow(int32_t w, int32_t h, const char* name);

    // void loop(int);

    // void Start();
    void init();
    void drawAll();

    // Shapes

    void drawTriangle(Point2D*, Color*);
    void drawRectangle(ANCHOR, Point2D* anch, int w, int h, int rot, Color*);
    void drawCircle(ANCHOR, Point2D* anch, int r, Color*);
    void drawLine(Point2D*, int w, Color*);
    
}

#endif