#ifndef VIZIOR_TEXTURE_H
#define VIZIOR_TEXTURE_H

#include <glad/glad.h>

#include "Common.hpp"

namespace vzr {

class Texture {
public:
    Texture(){}

    // If you are a user of the library you probably shouldn't call this function
    // When (Vizior's) texture objects are destroyed, the underlying opengl textures
    // are tracked but not deleted straight away, this function takes care of that.
    static void cleanDanglingTextures();
    
    
    static Texture* fromImage(const char* filepath);
    static Texture* fromGreyScale(int w, int h, unsigned char* data);
    static Texture* fromPixels(int w, int h, Color cols[]);
    ~Texture();

    unsigned int getID(){return m_TexID;}
    int getWidth(){return m_Width;}
    int getHeight(){return m_Height;}
    int getNrChannel(){return m_NrChannel;}
    //std::string getName(){return m_Name;}
private:
    unsigned int m_TexID;
    int m_Width, m_Height, m_NrChannel;
    //std::string m_Name;

};

}

#endif