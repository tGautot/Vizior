#ifndef VIZIOR_SHADERS_H
#define VIZIOR_SHADERS_H

#include <glad/glad.h>
#include <iostream>

namespace Vizior {

class Shader {
public:
    Shader(const char* vertCode, const char* fragCode);
    ~Shader();

    void use();
    unsigned int getId(){return m_Id;}
private:
    unsigned int m_Id;
};

}

#endif