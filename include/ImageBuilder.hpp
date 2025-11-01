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
#include <string>
#include <vector>
#include <cstdint>

#include "Common.hpp"
#include "Point2D.hpp"
#include "Texture.hpp"
#include "FontManager.hpp"
#include "Shaders.hpp"
#include "Camera.hpp"

// Maybe forward declaration of class is bad, read about it
class Window;

namespace vzr {

    typedef struct {
        GLenum mode;
        int start, cnt;
        double size; // Used for points and lines
        unsigned int shdrProg; // Used for tex and later for custom shaders
        int64_t tex_id; // Supposed to represent a GLid, which is unsigned int, here use sign to say if block uses tex or not
        //Texture* texture;
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

        void drawTriangle(const std::vector<Point2D>& points, const Color& color);
        void drawTriangle(const std::vector<Point2D>& points, const std::vector<Color>& colors);
        
        void drawRect(ANCHOR, const Point2D& anch, double width, double height, double rot, const Color& col);
        
        void drawQuad(const std::vector<Point2D>& pts, const Color& col);
        
        void drawGrid(const Point2D& top_left, double width, double height, Color* colors, int rows, int cols);
        
        // Draws the convex hull defined by the n points
        // TODO make it actual polygon, not convex hull
        void drawPolygon(const std::vector<Point2D>& pts, const Color& col);
        
        void drawCircle(const Point2D& center, double r, const Color& col);
        
        void drawEllipse(const Point2D& center, double rx, double ry, double rot, const Color& col);
        
        void drawRing(const Point2D& center, double inR, double outR, const Color& col);
        
        void drawArc(const Point2D& center, double r, double fromRad, double toRad, const Color& col);
        
        void drawRingArc(const Point2D& center, double inRadius, double outRadius, double fromRads, double toRads, const Color& col);
        
        void drawLine(const std::vector<Point2D>& points, double width, const Color& col, bool loop = false);
        
        void drawPoint(const Point2D& point, double size, const Color& col);
        
        void drawImage(ANCHOR, const Point2D& anch, Texture* image, double w, double h, double rot);
        
        void drawText(ANCHOR, const Point2D& anch, const std::string& text, const Color& col, const char* fontName, double scale, double rot);
        
        void drawBezier(const Point2D& p1, const Point2D& p2, const Point2D& c1, const Point2D& c2, const Color& col);

        Color getBackgroundColor(){return m_BgColor;}

        void setCamera(Camera* camSrc){m_Camera = camSrc;}
        void setBackgroundColor(Color col){m_BgColor = col;}
        
        Camera* getLatestCamera(){return m_Camera;}
        int getWidth(){return m_Width;}
        int getHeight(){return m_Height;}

        void submit();

        int addVert(double x, double y, const Color& color);
        int addVert(double x, double y, float s, float t);
        int addVert(double x, double y, const Color& color, float s, float t);
        void addElementBlock(GLenum mode, int vertexCount, double size, unsigned int shdrProg, int64_t tex);
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
        std::vector<float> m_Verts; // All vertex info (xyrgba or xyst) 
        std::vector<unsigned int> m_VertIdx; // Our EBO buffer
        int m_CurrElemID; // The number of complete vertex in the array

        std::vector<ElementBlock> m_ElemBlocks;

        unsigned int m_EBO, m_VAO, m_VBO;
        Shader *m_BaseShdr, *m_TexShdr, *m_GlyphShdr; 

    };
}

#endif
