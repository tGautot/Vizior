/**
 * @file ImageBuilder.hpp
 * @author Tom Gautot (tgautot.off@gmail.com)
 * @brief ImageBuilder serves as the portal between the programmer and OpenGL,
 * It is its only purpose, nothing window-related should be done here (for example, no include of GLFW here)  
 * @version 0.1
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef VIZIOR_IMAGE_BUILDER_HPP
#define VIZIOR_IMAGE_BUILDER_HPP

#include <glad/glad.h>
#include <math.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "Point2D.hpp"
#include "Texture.hpp"
#include "FontManager.hpp"
#include "Shaders.hpp"
#include "Camera.hpp"

// Maybe forward declaration of class is bad, read about it
class Window;

namespace Vizior {

    enum ANCHOR {
        ANCHOR_TR, ANCHOR_TL, ANCHOR_BR, ANCHOR_BL, ANCHOR_C // Top-Bottom Left-Right Center
    };

    typedef struct {
        uint8_t r, g, b, a;
    } Color;

    typedef struct {
        GLenum mode;
        int start, cnt;
        unsigned int size; // Used for points and lines
        unsigned int shdrProg; // Used for tex and later for custom shaders
        Texture* texture;
    } ElementBlock;


    extern Color RED;
    extern Color GRN;
    extern Color BLU;
    extern Color WHT;
    extern Color BLK;
    extern Color CYN;
    extern Color YLW;
    extern Color BRN;
    extern Color PRPL;
    extern Color ORG;
    extern Color GREY;
    extern Color DRK_RED;
    extern Color DRK_BLU;
    extern Color DRK_GRN;
    extern Color SKY_BLU;

    class ImageBuilder {
    friend class Window;
    public:
        ImageBuilder(int memSize = 20'000);
        ~ImageBuilder();

        void setDimensions(int w, int h);

        void clearAll();

        void drawTriangle(Point2D*, Color&);
        void drawRect(ANCHOR, Point2D& anch, int w, int h, float rot, Color&);
        void drawQuad(Point2D* pts, Color&);
        // Draws the convex hull defined by the n points
        // TODO make it actual polygon, not convex hull
        void drawPolygon(Point2D* pts, int n, Color&);
        void drawCircle(Point2D& center, int r, Color&);
        void drawEllipse(Point2D& center, int rx, int ry, int rot, Color&);
        void drawRing(Point2D& center, int inR, int outR, Color&);
        void drawArc(Point2D& center, int r, int from, int to, Color&);
        void drawRingArc(Point2D& center, int inR, int outR, int from, int to, Color&);
        void drawLine(Point2D*, int w, Color&);
        void drawLine(Point2D*, int n, int w, Color&, bool loop);
        void drawPoint(Point2D& point, unsigned int sz, Color&);
        void drawImage(ANCHOR, Point2D& anch, Texture* image, int w, int h, int rot);
        void drawText(ANCHOR, Point2D& anch, std::string text, Color& col, const char* fontName, float scale, int rot);
        void drawBezier(Point2D& p1, Point2D& p2, Point2D& c1, Point2D& c2, Color& col);

        Color getBackgroundColor(){return m_BgColor;}
        float* getVerts(){return m_Verts;}
        int getVertCount(){return m_NextVertPos;}
        unsigned int* getEBO(){return m_VertIdx;}
        int getElemCount(){return m_NextElemPos;}
        ElementBlock* getElemBlocks(){return m_ElemBlocks;}
        int getElemBlockCount(){return m_NextElemBlockPos;}

        void setCamera(Camera* camSrc){m_Camera = camSrc;}
        void setBackgroundColor(Color col){m_BgColor = col;}
        
        Camera* getLatestCamera(){return m_Camera;}
        int getWidth(){return m_Width;}
        int getHeight(){return m_Height;}
    protected:
        void submit();
    private:
        int addVert(int x, int y, Color& color);
        int addVert(int x, int y, float s, float t);
        int addVert(int x, int y, Color& color, float s, float t);
        void addElementBlock(GLenum mode, int vertexCount, unsigned int size, unsigned int shdrProg, Texture* tex);
        // TODO Make it so people that know how to write shader can modify the shaders used
        void compileBaseShaders();

        // OpenGL related values
        GLfloat m_LineWidthRange[2] = {0.0f, 0.0f};

        // Drawing space dimensions
        int m_Width, m_Height;

        Camera* m_Camera;
        Color m_BgColor;

        // For text rendering
        FontManager* m_FontManager;

        // Each "vertex" is xyrgba
        const int m_nVertexVals = 8; // number of vals in one "vertex" (xyrgbast)
        float* m_Verts; // All vertex info (xyrgba or xyst) 
        int m_NextVertPos; // Next pos to put vertex info, in above arrays
        unsigned int* m_VertIdx; // Our EBO buffer
        int m_NextElemPos; // Next pos to fill in m_VertIdx
        int m_CurrElemID; // The number of complete vertex in the array (should be nextpos/nVals)

        ElementBlock* m_ElemBlocks;
        int m_NextElemBlockPos;

        unsigned int m_EBO, m_VAO, m_VBO;
        Shader *m_BaseShdr, *m_TexShdr, *m_GlyphShdr; 

    };
}

#endif