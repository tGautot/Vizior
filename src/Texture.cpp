#include "Texture.hpp"

namespace Vizior {

Texture::Texture(const char* filepath){
    unsigned char *data = stbi_load(filepath, &m_Width, &m_Height, 
        &m_NrChannel, 0); 

    std::cout << "Got texture dim " << m_Width << "x" << m_Height << " and " << m_NrChannel << " channels" << std::endl; 
    m_Name = filepath;
    glGenTextures(1, &m_TexID);
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    if (data) {
        int mode = m_NrChannel == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, mode, m_Width, m_Height, 0, mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Is it really needed?
    }
    else {
        std::cout << "Failed to load texture " << m_Name << std::endl;
    }

    stbi_image_free(data);
}
}