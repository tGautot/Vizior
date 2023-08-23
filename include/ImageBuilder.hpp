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

// Maybe forward declaration of class is bad, read about it
class Window;

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

        float* getVerts(){return m_Verts;}
        int getVertCount(){return m_NextVertPos;}
        unsigned int* getEBO(){return m_VertIdx;}
        int getElemCount(){return m_NextElemPos;}

        int getWidth(){return m_Width;}
        int getHeight(){return m_Height;}
    protected:
        void submit();
    private:
        int addVert(int x, int y, Color& color);
        // TODO Make it so people that know how to write shader can modify the shaders used
        // TODO Should probably put shaders somewhere else than directly in window too
        void compileShaders();
        
        int m_Width, m_Height;

        // Each "vertex" is xyrgba
        const int m_nVertexVals = 6; // number of vals in one "vertex"
        float* m_Verts; 
        int m_NextVertPos;
        unsigned int* m_VertIdx; // Our EBO buffer
        int m_NextElemPos;

        unsigned int m_ShaderProgram, m_EBO, m_VAO, m_VBO;
        const char* m_VertexShaderSrc = 
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "out vec4 vertexColor;\n"
            "void main()\n"
            "{ vertexColor = aColor;"
            "gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);}\n\0";
    
        const char* m_FragmentShaderSrc =
            "#version 330 core\n"
            "in vec4 vertexColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{ FragColor = vertexColor; }\n\0";
    };
}

#endif