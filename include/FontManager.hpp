#ifndef VIZIOR_FONT_MANAGER_H
#define VIZIOR_FONT_MANAGER_H

#include <map>
#include <utility> // for std::pair
#include <freetype/freetype.h>
#include <cstring>
#include "Texture.hpp"

namespace vzr {

struct GlyphDescriptor {
    unsigned char c;
    const char* fontName;

};

// from https://learnopengl.com/In-Practice/Text-Rendering
// Go there to know what size-bearing-advance are
struct Glyph {
    Texture* tex;  // ID handle of the glyph texture
    int w,h;       // Size of glyph
    int bmL, bmT;  // Offset from baseline to left/top of glyph
    long int advance;    // Offset to advance to next glyph
};

class FontManager {
public:
    static FontManager* getInstance();

    int registerFont(const char* fontPath, const char* name);
    Glyph* getGlyph(char c, const char* fontName);
private: 

    FontManager(){}
    FontManager(const FontManager& o){}

    static FontManager* instance;
    FT_Library ft;
    std::map<GlyphDescriptor, Glyph> glyphMap;
};


}

#endif