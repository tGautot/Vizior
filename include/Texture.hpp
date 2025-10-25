#ifndef VIZIOR_TEXTURE_H
#define VIZIOR_TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <string>

namespace vzr {

class Texture {
public:
    Texture(){}
    static Texture* fromImage(const char* filepath);
    static Texture* fromGreyScale(int w, int h, unsigned char* data);
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