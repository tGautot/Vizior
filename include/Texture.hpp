#ifndef VIZIOR_TEXTURE_H
#define VIZIOR_TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

namespace Vizior {

class Texture {
    public:
        Texture(const char* filepath);
        ~Texture();

        unsigned int getID(){return m_TexID;}
        int getWidth(){return m_Width;}
        int getHeight(){return m_Height;}
        int getNrChannel(){return m_NrChannel;}
        const char* getName(){return m_Name;}
    private:
        unsigned int m_TexID;
        int m_Width, m_Height, m_NrChannel;
        const char* m_Name;

};

}

#endif