#ifndef VIZIOR_IMAGE_BUILDER_HPP
#define VIZIOR_IMAGE_BUILDER_HPP
#include <math.h>
#include <cstdint>
#include "Point2D.hpp"

namespace Vizior {

    enum ANCHOR {
        ANCHOR_TR, ANCHOR_TL, ANCHOR_BR, ANCHOR_BL, ANCHOR_C // Top-Bottom Left-Right Center
    };

    typedef struct {
        uint8_t r, g, b, a;
    } Color;


    extern Color RED;
    extern Color GRN;
    extern Color BLU;
    extern Color WHT;
    extern Color BLK;

    class ImageBuilder {
    public:
        ImageBuilder();
        ~ImageBuilder();

        void setDimensions(int w, int h);

        void clearAll();

        void drawTriangle(Point2D*, Color&);
        void drawRectangle(ANCHOR, Point2D& anch, int w, int h, float rot, Color&);
        void drawCircle(Point2D& anch, int r, Color&);
        void drawLine(Point2D*, int w, Color&);

        float* getVerts(){return m_Verts;}
        int getVertCount(){return m_NextVertPos;}
        unsigned int* getEBO(){return m_VertIdx;}
        int getElemCount(){return m_NextElemPos;}

        int getWidth(){return m_Width;}
        int getHeight(){return m_Height;}
    private:
        int addVert(int x, int y, Color& color);

        int m_Width, m_Height;

        // Each "vertex" is xyrgba
        const int m_nVertexVals = 6; // number of vals in one "vertex"

        float* m_Verts; 
        int m_NextVertPos;
        unsigned int* m_VertIdx; // Our EBO buffer
        int m_NextElemPos;
    };
}

#endif