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

#include "Camera.hpp"



namespace vzr {
    
    // Just to hide GLFW api
    enum Key {
        KEY_A = GLFW_KEY_A,
        KEY_B = GLFW_KEY_B,
        KEY_C = GLFW_KEY_C,
        KEY_D = GLFW_KEY_D,
        KEY_E = GLFW_KEY_E,
        KEY_F = GLFW_KEY_F,
        KEY_G = GLFW_KEY_G,
        KEY_H = GLFW_KEY_H,
        KEY_I = GLFW_KEY_I,
        KEY_J = GLFW_KEY_J,
        KEY_K = GLFW_KEY_K,
        KEY_L = GLFW_KEY_L,
        KEY_M = GLFW_KEY_M,
        KEY_N = GLFW_KEY_N,
        KEY_O = GLFW_KEY_O,
        KEY_P = GLFW_KEY_P,
        KEY_Q = GLFW_KEY_Q,
        KEY_R = GLFW_KEY_R,
        KEY_S = GLFW_KEY_S,
        KEY_T = GLFW_KEY_T,
        KEY_U = GLFW_KEY_U,
        KEY_V = GLFW_KEY_V,
        KEY_W = GLFW_KEY_W,
        KEY_X = GLFW_KEY_X,
        KEY_Y = GLFW_KEY_Y,
        KEY_z = GLFW_KEY_Z,
        KEY_1 = GLFW_KEY_1,
        KEY_2 = GLFW_KEY_2,
        KEY_3 = GLFW_KEY_3,
        KEY_4 = GLFW_KEY_4,
        KEY_5 = GLFW_KEY_5,
        KEY_6 = GLFW_KEY_6,
        KEY_7 = GLFW_KEY_7,
        KEY_8 = GLFW_KEY_8,
        KEY_9 = GLFW_KEY_9,
        KEY_0 = GLFW_KEY_0,
        UP = GLFW_KEY_UP,
        DOWN = GLFW_KEY_DOWN,
        LEFT = GLFW_KEY_LEFT,
        RIGHT = GLFW_KEY_RIGHT,
    };





    class Window 
        : public std::enable_shared_from_this<Window> {
    friend class ImageBuilder;
    public:
        Window(int w, int h, const char* name);
        ~Window();
        void drawSource();
        void setSource(std::shared_ptr<ImageBuilder> src);


        Point2D getMouseWindowPos();
        Point2D getMouseWorldPos();
        bool isMousePressed();
        bool isKeyPressed(int key);

        std::shared_ptr<ImageBuilder> getSource(){return m_Src;}
        int getWidth(){return this->m_Width;}
        int getHeight(){return this->m_Height;}
        void updateCamPos();
        void scrollCallback(double horScroll, double vertScroll);

        bool shouldClose();
        //std::shared_ptr<Window> getSharedPtr(){return shared_from_this();}

        void setCameraEnabled(bool val){m_CameraEnabled = val;};
        void resetCamera();

    private:
        
        std::shared_ptr<ImageBuilder> m_Src = nullptr;

        Camera* m_Camera;

        unsigned int m_WinId;
        const char* m_WinName;  
        int m_Width, m_Height;
        GLFWwindow* m_glfw_Window;

        bool m_CameraEnabled;

    };

}



#endif