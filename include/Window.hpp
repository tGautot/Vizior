/**
 * @file Window.hpp
 * @author Tom Gautot (tgautot.off@gmail.com)
 * @brief Window abstracts windows to the programmer, the simple fact of creating this object
 * should open a window
 * @version 0.1
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef VIZIOR_WINDOW_H
#define VIZIOR_WINDOW_H

// ImageBuilder imports glad, which need to be imported before GLFW  
#include "ImageBuilder.hpp" 
// ----------------------------------------

#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <string>

namespace Vizior {

    class Window 
        : public std::enable_shared_from_this<Window> {
    friend class ImageBuilder;
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
        
        
        std::shared_ptr<ImageBuilder> m_Src = nullptr;

        
        unsigned int m_WinId;
        const char* m_WinName;  
        int m_Width, m_Height;
        GLFWwindow* m_glfw_Window;

    };

}



#endif