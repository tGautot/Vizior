#include "FontManager.hpp"

namespace Vizior {

FontManager* FontManager::instance = nullptr;

FontManager* FontManager::getInstance(){
    if(FontManager::instance == nullptr){
        FontManager::instance = new FontManager();

        if (FT_Init_FreeType(&(instance->ft))) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return nullptr;
        }

    }
    return instance;
}

int FontManager::registerFont(const char* fontPath, const char* fontName){
    FT_Face face;
    std::cout << "Creating new face" << std::endl;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return -1;
    }
    std::cout << "Setting pixel size" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48); // TODO put height as parameter

    for (unsigned char c = 0; c < 128; c++){
        // load character glyph 

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        Texture* tex = Texture::fromGreyScale(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

        Glyph glyph = {
            tex, 
            tex->getWidth(), tex->getHeight(),
            face->glyph->bitmap_left, face->glyph->bitmap_top,
            face->glyph->advance.x
        };

        GlyphDescriptor gd = {
            c, fontName
        };
        
        glyphMap.insert(std::pair<GlyphDescriptor, Glyph>(gd, glyph));
    }

    FT_Done_Face(face);
    return 0;
}

Glyph* FontManager::getGlyph(char c, const char* fontName){
    GlyphDescriptor gd{(unsigned char)c, fontName};
    return &(glyphMap[gd]);
}

bool operator<(const GlyphDescriptor &lhs, const GlyphDescriptor &rhs)
{
    int cmp = strcmp(lhs.fontName, rhs.fontName);
    if(cmp == 0){
        return lhs.c < rhs.c;
    }
    return cmp < 0;
}

}