#ifndef VIZIOR_WINDOW_H
#define VIZIOR_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <string>
#include "ImageBuilder.hpp"

namespace Vizior {

    class Window 
        : public std::enable_shared_from_this<Window> {
    public:
        Window(int w, int h, const char* name);
        ~Window();
        void drawSource();
        void setSource(std::shared_ptr<ImageBuilder> src);

        std::shared_ptr<ImageBuilder> getSource(){return m_Src;}
        int getWidth(){return this->m_Width;}
        int getHeight(){return this->m_Height;}

        bool shouldClose();
        //std::shared_ptr<Window> getSharedPtr(){return shared_from_this();}
    private:
        // Make it so people that know how to write shader can modify the shaders used
        // Should probably put shaders somewhere else than directly in window too
        void compileShaders();
        
        std::shared_ptr<ImageBuilder> m_Src = nullptr;

        unsigned int m_ShaderProgram, m_EBO, m_VAO, m_VBO;
        unsigned int m_WinId;
        const char* m_WinName;  
        int m_Width, m_Height;
        GLFWwindow* m_glfw_Window;

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