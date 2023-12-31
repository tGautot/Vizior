#include "Texture.hpp"

namespace Vizior {

Texture* Texture::fromImage(const char* filepath){
    Texture* tex = new Texture();
    
    unsigned char *data = stbi_load(filepath, &(tex->m_Width), &(tex->m_Height), 
        &(tex->m_NrChannel), 0); 

    std::cout << "Got texture dim " << tex->m_Width << "x" << tex->m_Height << " and " << tex->m_NrChannel << " channels" << std::endl; 
    //m_Name = filepath;
    glGenTextures(1, &(tex->m_TexID));
    glBindTexture(GL_TEXTURE_2D, tex->m_TexID);


    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        int mode = tex->m_NrChannel == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, mode, tex->m_Width, tex->m_Height, 0, mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Is it really needed?
    }
    else {
        std::cout << "Failed to load texture " /* << m_Name */ << std::endl;
    }

    stbi_image_free(data);
    return tex;
}


Texture* Texture::fromGreyScale(int w, int h, unsigned char* data){
    Texture* tex = new Texture();
    tex->m_Width = w; tex->m_Height = h; tex->m_NrChannel = 1;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable alignment
    glGenTextures(1, &tex->m_TexID);
    glBindTexture(GL_TEXTURE_2D, tex->m_TexID);
    
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int mode = GL_RED;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Is it really needed?

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // enable alignment
    return tex;
}
}