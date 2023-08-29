#ifndef VIZIOR_H
#define VIZIOR_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <unistd.h> // for sleep
#include <stb_image.h>
#include "ImageBuilder.hpp"
#include "Window.hpp"


namespace Vizior {

    typedef void (*loop_func)(int);
    
    extern loop_func loop;
    extern int16_t targetFrameRate;
    extern std::vector<std::shared_ptr<Window>> windows;
    
    unsigned int registerWindow(std::shared_ptr<Window> win);
    unsigned int registerFont(const char* path);
    void setLoopFunc(loop_func loop);
    void Start();
    
}

#endif /* !VIZIOR_H */