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
#include "Point2D.hpp"
#include "Texture.hpp"

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

    class ImageBuilder {
    friend class Window;
    public:
        ImageBuilder();
        ~ImageBuilder();

        void setDimensions(int w, int h);

        void clearAll();

        void drawTriangle(Point2D*, Color&);
        void drawRectangle(ANCHOR, Point2D& anch, int w, int h, float rot, Color&);
        void drawCircle(Point2D& anch, int r, Color&);
        void drawLine(Point2D*, int w, Color&);
        void drawPoint(Point2D& point, unsigned int sz, Color&);
        void drawImage(ANCHOR, Point2D& anch, Texture* image, int w, int h, int rot);

        float* getVerts(){return m_Verts;}
        int getVertCount(){return m_NextVertPos;}
        unsigned int* getEBO(){return m_VertIdx;}
        int getElemCount(){return m_NextElemPos;}
        ElementBlock* getElemBlocks(){return m_ElemBlocks;}
        int getElemBlockCount(){return m_NextElemBlockPos;}

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
        // TODO Should probably put shaders somewhere else than directly in window too
        void compileShaders();

        // OpenGL related values
        GLfloat m_LineWidthRange[2] = {0.0f, 0.0f};

        // Drawing space dimensions
        int m_Width, m_Height;

        // Each "vertex" is xyrgba
        const int m_nVertexVals = 8; // number of vals in one "vertex" (xyrgbast)
        float* m_Verts; // All vertex info (xyrgba or xyst) 
        int m_NextVertPos; // Next pos to put vertex info, in above arrays
        unsigned int* m_VertIdx; // Our EBO buffer
        int m_NextElemPos; // Next pos to fill in m_VertIdx
        int m_CurrElemID; // The number of complete vertex in the array (should be nextpos/nVals)

        ElementBlock* m_ElemBlocks;
        int m_NextElemBlockPos;

        unsigned int m_ShaderProgram, m_EBO, m_VAO, m_VBO;
        const char* m_VertexShaderSrc = 
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "layout (location = 2) in vec2 aTexPos;\n"
            "out vec4 vertexColor;\n"
            "out vec2 texPos;\n"
            "void main()\n"
            "{ vertexColor = aColor; texPos = aTexPos;\n"
            "gl_Position = vec4(aPos.xy, 0.0, 1.0);}\n\0";
    
        const char* m_FragmentShaderSrc =
            "#version 330 core\n"
            "in vec4 vertexColor;\n"
            "in vec2 texPos;\n"
            "out vec4 FragColor;\n"
            "uniform sampler2D boundTex;\n"
            "uniform float useTex;\n"
            "void main()\n"
            "{ FragColor = abs(useTex-1.0)*vertexColor + useTex * texture(boundTex, texPos); }\n\0";


    };
}

#endif