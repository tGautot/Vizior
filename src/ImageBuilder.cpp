#include "ImageBuilder.hpp"

namespace Vizior {

Color RED = {255,0,0,255};
Color GRN = {0,255,0,255};
Color BLU = {0,0,255,255};
Color WHT = {255,255,255,255};
Color BLK = {0,0,0,1};

ImageBuilder::ImageBuilder(){
    // At most 10000 vertices
    // TODO change this or handle it better
    m_Verts = new float[m_nVertexVals*10000];
    m_VertIdx = new unsigned int[3*10000];
    m_ElemBlocks = new ElementBlock[1*10000];
    m_NextElemPos = 0;
    m_NextVertPos = 0;
    m_NextElemBlockPos = 0;
    m_CurrElemID = 0;

    m_FontManager = FontManager::getInstance();

    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, m_LineWidthRange);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Max pure ogl line width is " << m_LineWidthRange[1] << std::endl;
    // Will be set for real once bound to a window, polly not needed
    m_Width = 100;
    m_Height = 100;
    compileBaseShaders();
    m_BaseShdr->use();
}

void ImageBuilder::compileBaseShaders(){
    const char* vertexShaderSrc = 
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec4 aColor;\n"
        "layout (location = 2) in vec2 aTexPos;\n"
        "out vec4 vertexColor;\n"
        "out vec2 texPos;\n"
        "uniform vec4 camProps;\n" // xy is camPos and zw is camSize
        "void main()\n"
        "{ vertexColor = aColor; texPos = aTexPos;\n"
        "vec2 xy = (aPos - camProps.xy)/(camProps.zw/2);\n"
        "gl_Position = vec4(xy, 0.0, 1.0);}\n\0";

    const char* baseFragmentShaderSrc =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "in vec2 texPos;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{ FragColor = vertexColor; }\n\0";

    const char* texFragmentShaderSrc =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "in vec2 texPos;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D boundTex;\n"
        "void main()\n"
        "{ FragColor = texture(boundTex, texPos); }\n\0";

    const char* textFragmentShaderSrc =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "in vec2 texPos;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D boundTex;\n"
        "void main()\n"
        "{ FragColor = vec4(vertexColor.rgb, vertexColor.a * texture(boundTex, texPos).r); }\n\0";

    m_BaseShdr = new Shader(vertexShaderSrc, baseFragmentShaderSrc);
    m_TexShdr = new Shader(vertexShaderSrc, texFragmentShaderSrc);
    m_GlyphShdr = new Shader(vertexShaderSrc, textFragmentShaderSrc);

}

ImageBuilder::~ImageBuilder(){
    delete[] m_Verts;
    delete[] m_VertIdx;
    delete[] m_ElemBlocks;
    delete m_BaseShdr;
    delete m_TexShdr;
    delete m_GlyphShdr;
}

int ImageBuilder::addVert(int x, int y, Color& color){
    return addVert(x,y,color,0.0f,0.0f);
}

Color nilColor = {0,0,0,0};
int ImageBuilder::addVert(int x, int y, float s, float t){
    return addVert(x,y,nilColor,s,t);
}

int ImageBuilder::addVert(int x, int y, Color& color, float s, float t){
    m_Verts[m_NextVertPos++] = (1.0f * x);
    m_Verts[m_NextVertPos++] = (1.0f * y);
    m_Verts[m_NextVertPos++] = color.r/255.0;
    m_Verts[m_NextVertPos++] = color.g/255.0;
    m_Verts[m_NextVertPos++] = color.b/255.0;
    m_Verts[m_NextVertPos++] = color.a/255.0;
    m_Verts[m_NextVertPos++] = s;
    m_Verts[m_NextVertPos++] = t;
    return m_CurrElemID++;
}

void ImageBuilder::addElementBlock(GLenum mode, int elemCount, unsigned int size, unsigned int shdrProg, Texture* tex){
    if(size < 1) size = 1;
    if(m_NextElemBlockPos == 0){
        m_ElemBlocks[0] = {mode, 0, elemCount, size, shdrProg, tex};
        m_NextElemBlockPos++;
        return;
    }
    ElementBlock lastBlock = m_ElemBlocks[m_NextElemBlockPos-1];
    GLenum lastMode = lastBlock.mode;
    if(lastMode == mode && (lastMode == GL_TRIANGLES || lastMode == GL_POINTS) && lastBlock.size == size 
            && lastBlock.shdrProg == shdrProg && lastBlock.texture == tex){
        m_ElemBlocks[m_NextElemBlockPos-1].cnt += elemCount;
        return;
    }
    m_ElemBlocks[m_NextElemBlockPos] = {mode, lastBlock.start + lastBlock.cnt, elemCount, size, shdrProg, tex};
    m_NextElemBlockPos++;
}

void ImageBuilder::clearAll(){
    m_NextElemPos = 0;
    m_NextVertPos = 0;
    m_NextElemBlockPos = 0;
    m_CurrElemID = 0;
}

void ImageBuilder::drawTriangle(Point2D* ps, Color& color){
    for(int i = 0; i < 3; i++){
        m_VertIdx[m_NextElemPos] = addVert(ps[i].x, ps[i].y, color);
        m_NextElemPos++;
    }
    addElementBlock(GL_TRIANGLES, 3, 0.0f, m_BaseShdr->getId(), nullptr);
}

Point2D* getCornersOfRect(ANCHOR a, Point2D& anch, int w, int h, int rot){
    Point2D* corners = new Point2D[4]; 
    Point2D *bl = corners, *tl = corners+1 , *br = corners+2, *tr = corners+3;

    switch(a){
        case ANCHOR_C:
            *bl = Point2D(anch.x - w/2, anch.y - h/2);
            *tl = Point2D(anch.x - w/2, anch.y + h/2);
            *br = Point2D(anch.x + w/2, anch.y - h/2);
            *tr = Point2D(anch.x + w/2, anch.y + h/2);
            break;
        case ANCHOR_BL:
            *bl = Point2D(anch.x  , anch.y  );
            *tl = Point2D(anch.x  , anch.y+h);
            *br = Point2D(anch.x+w, anch.y  );
            *tr = Point2D(anch.x+w, anch.y+h);
            break;
        case ANCHOR_TL:
            *bl = Point2D(anch.x  , anch.y-h);
            *tl = Point2D(anch.x  , anch.y  );
            *br = Point2D(anch.x+w, anch.y-h);
            *tr = Point2D(anch.x+w, anch.y  );
            break;
        case ANCHOR_BR:
            *bl = Point2D(anch.x-w, anch.y  );
            *tl = Point2D(anch.x-w, anch.y+h);
            *br = Point2D(anch.x  , anch.y  );
            *tr = Point2D(anch.x  , anch.y+h);
            break;
        case ANCHOR_TR:
            *bl = Point2D(anch.x-w, anch.y-h);
            *tl = Point2D(anch.x-w, anch.y  );
            *br = Point2D(anch.x  , anch.y-h);
            *tr = Point2D(anch.x  , anch.y  );
            break;
    }

    *bl = bl->rotateAroundBy(anch, rot);
    *tl = tl->rotateAroundBy(anch, rot);
    *br = br->rotateAroundBy(anch, rot);
    *tr = tr->rotateAroundBy(anch, rot);
    return corners;
}

void ImageBuilder::drawRectangle(ANCHOR a, Point2D& anch, int w, int h, float rot, Color& color){
    Point2D* corners = getCornersOfRect(a, anch, w, h, rot);
    Point2D bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

    int blIdx = addVert(bl.x, bl.y, color); 
    int tlIdx = addVert(tl.x, tl.y, color); 
    int brIdx = addVert(br.x, br.y, color); 
    int trIdx = addVert(tr.x, tr.y, color); 

    delete[] corners;
    
    m_VertIdx[m_NextElemPos++] = blIdx;
    m_VertIdx[m_NextElemPos++] = tlIdx;
    m_VertIdx[m_NextElemPos++] = brIdx;
    m_VertIdx[m_NextElemPos++] = trIdx;
    m_VertIdx[m_NextElemPos++] = brIdx;
    m_VertIdx[m_NextElemPos++] = tlIdx;


    addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_BaseShdr->getId(), nullptr);
}

// TODO better ways to draw cricle probably
void ImageBuilder::drawCircle(Point2D& center, int r, Color& color){
    // Center point
    int startElemId = addVert(center.x, center.y, color);

    int precision = 180;
    // Circle points
    int last = -1, beforeLast = -1, first=-1;
    for(int i = 0; i < precision; i++){
        int px = center.x + cos(M_PI * i*2 / precision) * r;
        int py = center.y + sin(M_PI * i*2 / precision) * r;
        beforeLast = last;
        last = addVert(px, py, color);
        if(i >= 1){
            m_VertIdx[m_NextElemPos++] = startElemId;
            m_VertIdx[m_NextElemPos++] = beforeLast;
            m_VertIdx[m_NextElemPos++] = last;
        } 
        if(i == 0){ first = last;}

    }
    m_VertIdx[m_NextElemPos++] = startElemId;
    m_VertIdx[m_NextElemPos++] = last;
    m_VertIdx[m_NextElemPos++] = first;
    addElementBlock(GL_TRIANGLES, precision*3, 0.0f, m_BaseShdr->getId(), nullptr);

}

void ImageBuilder::drawLine(Point2D* ps, int w, Color& color){
    if(w > m_LineWidthRange[1]){
        // Requested line width is too high for GPU, simulate it with rectangle
        Point2D center = (ps[0] + ps[1])/2;
        Point2D orientation = ps[1] - ps[0];
        float angle = 180 * atan2(orientation.y, orientation.x) / M_PI;
        
        int length = sqrt(orientation.x*orientation.x + orientation.y*orientation.y);
        drawRectangle(ANCHOR_C, center, length, w, angle, color);
        return;
    }
    // Pure OpenGL line
    m_VertIdx[m_NextElemPos++] = addVert(ps[0].x, ps[0].y, color);
    m_VertIdx[m_NextElemPos++] = addVert(ps[1].x, ps[1].y, color);
    addElementBlock(GL_LINE_STRIP, 2, w, m_BaseShdr->getId(), nullptr);
}

void ImageBuilder::drawLine(Point2D* ps, int n, int w, Color& color, bool loop){
    if(n <= 2) {drawLine(ps, w, color);}
    if(w > m_LineWidthRange[1]){
        // Requested line width is too high for GPU
        // Since its a strip, cant really do rectangle
        std::cout << "ERROR::DRAW_LINE(n>2)::Width too big, max is " << m_LineWidthRange[1] << std::endl; 
        return;
    }
    // Pure OpenGL line
    for(int i = 0; i < n; i++){
        m_VertIdx[m_NextElemPos++] = addVert(ps[i].x, ps[i].y, color);
    }
    addElementBlock(loop ? GL_LINE_LOOP : GL_LINE_STRIP, n, w, m_BaseShdr->getId(), nullptr);
}


void ImageBuilder::drawPoint(Point2D& pt, unsigned int sz, Color& color){
    m_VertIdx[m_NextElemPos++] = addVert(pt.x, pt.y, color);
    addElementBlock(GL_POINTS, 1, sz, m_BaseShdr->getId(), nullptr);
}

void ImageBuilder::drawImage(ANCHOR a, Point2D& anch, Texture* image, int w, int h, int rot){
    Point2D* corners = getCornersOfRect(a, anch, w, h, rot);
    Point2D bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

    int blIdx = addVert(bl.x, bl.y, 0.0f, 0.0f); 
    int tlIdx = addVert(tl.x, tl.y, 0.0f, 1.0f); 
    int brIdx = addVert(br.x, br.y, 1.0f, 0.0f); 
    int trIdx = addVert(tr.x, tr.y, 1.0f, 1.0f); 

    delete[] corners;

    m_VertIdx[m_NextElemPos++] = blIdx;
    m_VertIdx[m_NextElemPos++] = tlIdx;
    m_VertIdx[m_NextElemPos++] = brIdx;
    m_VertIdx[m_NextElemPos++] = trIdx;
    m_VertIdx[m_NextElemPos++] = brIdx;
    m_VertIdx[m_NextElemPos++] = tlIdx;

    addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_TexShdr->getId(), image);
}

void ImageBuilder::drawText(ANCHOR a, Point2D& anch, std::string text, Color& col, const char* fontName, float scale, int rot){
    // Code mostly copied from https://learnopengl.com/In-Practice/Text-Rendering

    // iterate through all characters
    std::string::const_iterator c;
    int x = anch.x, y = anch.y, blIdx, tlIdx, brIdx, trIdx;
    Point2D bl, tl, br, tr, nxtPos;
    Point2D* corners;
    for (c = text.begin(); c != text.end(); c++)
    {
        Glyph* glyph = m_FontManager->getGlyph(*c, fontName);

        int xpos = x + (int)(glyph->bmL * scale);
        int ypos = y - (int)(glyph->h - glyph->bmT * scale);

        int w = (int)(glyph->w * scale);
        int h = (int)(glyph->h * scale);
        nxtPos = {xpos, ypos};
        corners = getCornersOfRect(ANCHOR_BL, nxtPos, w, h, rot);
        bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

        blIdx = addVert(bl.x, bl.y, col, 0.0f, 1.0f); 
        tlIdx = addVert(tl.x, tl.y, col, 0.0f, 0.0f); 
        brIdx = addVert(br.x, br.y, col, 1.0f, 1.0f); 
        trIdx = addVert(tr.x, tr.y, col, 1.0f, 0.0f); 

        delete[] corners;

        m_VertIdx[m_NextElemPos++] = blIdx;
        m_VertIdx[m_NextElemPos++] = tlIdx;
        m_VertIdx[m_NextElemPos++] = brIdx;
        m_VertIdx[m_NextElemPos++] = trIdx;
        m_VertIdx[m_NextElemPos++] = brIdx;
        m_VertIdx[m_NextElemPos++] = tlIdx;


        addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_GlyphShdr->getId(), glyph->tex);
        
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (glyph->advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}


void ImageBuilder::setDimensions(int w, int h){
    m_Width = w;
    m_Height = h;
}

void ImageBuilder::submit(){   

    glGenVertexArrays(1,&m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    //std::cout << "Src got " << getVertCount() << " Vertices and " << getElemCount() << " Elems" << std::endl;
    //std::cout << "Src dims " << getWidth() << " x " << getHeight() << " px" << std::endl;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, getVertCount()*sizeof(float), getVerts(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getElemCount()*sizeof(unsigned int), getEBO(), GL_STATIC_DRAW);

    // Function gives info about the vertex in the "vertices" array and how they should be used
    // for the vertex shader
    // First param is index of vertex attribute (in case your vertex buffer contains many things,
    // like coordinates, as weel as color, it is linked to the "location" var in the shader code
    // we set location = 0 so we must put 0 here)
    // The shader takes vertices 3 by 3 (2nd param)
    // Values are floats
    // Should NOT be normalized (clamp to [-1, 1] or [0,1])
    // 2 vertices are separated by 20 bytes
    // first vertex starts at position 0 (last param)

    // No-Texture vertex attrib
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, m_nVertexVals*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, m_nVertexVals*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_nVertexVals*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int currProgram = m_BaseShdr->getId();
    bool currWithTex = false;
    glUseProgram(currProgram);
    glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    int nBlock = getElemBlockCount();
    //std::cout << "There are " << nBlock << " elem blocks" << std::endl;

    Point2D camPos = m_Camera->getPos();
    float camZoom = m_Camera->getZoom();
    std::cout << "Camera has pos " << camPos.x << "," << camPos.y <<  " and zoom factor " << camZoom << std::endl;
    
    for(int i = 0; i < nBlock; i++){
        ElementBlock block = m_ElemBlocks[i];
        
        //std::cout << "Drawing block of mode " << block.mode << " from " << block.start << " of " << block.cnt << " elements, size " << block.size << " and texture " << block.texture << std::endl;

        if(block.shdrProg != currProgram){
            glUseProgram(block.shdrProg);
            currProgram = block.shdrProg;
            int loc = glGetUniformLocation(currProgram, "camProps");
            glUniform4f(loc, camPos.x, camPos.y, m_Width/camZoom, m_Height/camZoom);
        }

        if(block.texture == nullptr){
            currWithTex = false;
        } else {
            currWithTex = true;
            glBindTexture(GL_TEXTURE_2D, block.texture->getID());
        }

        if(block.mode == GL_LINES || block.mode == GL_LINE_STRIP || block.mode == GL_LINE_LOOP){
            glLineWidth(block.size*camZoom);
        } else if(block.mode == GL_POINTS){
            glPointSize(block.size*camZoom);
        }
        glDrawElements(block.mode, block.cnt, GL_UNSIGNED_INT, (void*)(block.start*sizeof(unsigned int)));        
    }
    
    // TODO put this somewhere else, here prevents diff windows from having same src
    clearAll();

}



}