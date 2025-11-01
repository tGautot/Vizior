#include "ImageBuilder.hpp"

#define PB(vec, val) vec.push_back(val)

namespace vzr {

Color RED = {255,0,0,255};
Color GRN = {0,255,0,255};
Color BLU = {0,0,255,255};
Color WHT = {255,255,255,255};
Color BLK = {0,0,0,1};
Color CYN = {0,255,255,255};
Color YLW = {255,255,0,255};
Color PRPL = {255,0,255,255};
Color BRN = {100,50,0,255};
Color ORG = {255,128,0,255};
Color GREY = {128,128,128,255};
Color DRK_RED = {100,0,0,255};
Color DRK_BLU = {0,100,0,255};
Color DRK_GRN = {0,0,100,255};
Color SKY_BLU = {100,170,255,255};

ImageBuilder::ImageBuilder(int size){
    m_Verts.reserve(m_nVertexVals*size);
    m_VertIdx.reserve(3*size);
    m_ElemBlocks.reserve(3*size);
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
    m_BgColor = {255,255,255,255};
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
        "uniform mat4 viewMat;\n" 
        "uniform vec2 vertSpace;\n" // WidthxHeight to map back to -1 1
        "void main()\n"
        "{ vertexColor = aColor; texPos = aTexPos;\n"
        "gl_Position = (viewMat * vec4(aPos, 0.0, 1.0))/vec4(vertSpace/2, 1.0, 1.0);}\n\0";

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
    delete m_BaseShdr;
    delete m_TexShdr;
    delete m_GlyphShdr;
}

int ImageBuilder::addVert(double x, double y, const Color& color){
    return addVert(x,y,color,0.0f,0.0f);
}

Color nilColor = {0,0,0,0};
int ImageBuilder::addVert(double x, double y, float s, float t){
    return addVert(x,y,nilColor,s,t);
}

int ImageBuilder::addVert(double x, double y, const Color& color, float s, float t){
    PB(m_Verts, x);
    PB(m_Verts, y);
    PB(m_Verts, color.r/255.0);
    PB(m_Verts, color.g/255.0);
    PB(m_Verts, color.b/255.0);
    PB(m_Verts, color.a/255.0);
    PB(m_Verts, s);
    PB(m_Verts, t);
    return m_CurrElemID++;
}

void ImageBuilder::addElementBlock(GLenum mode, int elemCount, double size, unsigned int shdrProg, int64_t tex_id){
    if(size < 1) size = 1;
    if(m_ElemBlocks.size() == 0){
        m_ElemBlocks.push_back({mode, 0, elemCount, size, shdrProg, tex_id});
        return;
    }
    ElementBlock& lastBlock = m_ElemBlocks.back();
    GLenum lastMode = lastBlock.mode;
    if(lastMode == mode && (lastMode == GL_TRIANGLES || lastMode == GL_POINTS) && lastBlock.size == size 
            && lastBlock.shdrProg == shdrProg && lastBlock.tex_id == tex_id){
        lastBlock.cnt += elemCount;
        return;
    }
    m_ElemBlocks.push_back({mode, lastBlock.start + lastBlock.cnt, elemCount, size, shdrProg, tex_id});
}

void ImageBuilder::clearAll(){
    m_CurrElemID = 0;
    m_ElemBlocks.clear();
    m_Verts.clear();
    m_VertIdx.clear();
}

void ImageBuilder::drawTriangle(const std::vector<Point2D>& ps, const Color& color){
    if(ps.size() < 3){
        std::cout << "WARNING::DRAW_TRANGLE Need at least 3 points but only " << ps.size() << " given, skipping..." << std::endl; 
        return;
    }
    for(int i = 0; i < 3; i++){
        m_VertIdx.push_back(addVert(ps[i].x, ps[i].y, color));
    }
    addElementBlock(GL_TRIANGLES, 3, 0.0f, m_BaseShdr->getId(), -1);
}

void ImageBuilder::drawTriangle(const std::vector<Point2D>& ps, const std::vector<Color>& colors){
    if(ps.size() < 3){
        std::cout << "WARNING::DRAW_TRANGLE Need at least 3 points but only " << ps.size() << " given, skipping..." << std::endl; 
        return;
    }
    if(colors.size() < 3){
        std::cout << "WARNING::DRAW_TRANGLE Need at least 3 colors but only " << colors.size() << " given, skipping..." << std::endl; 
        return;
    }
    for(int i = 0; i < 3; i++){
        m_VertIdx.push_back(addVert(ps[i].x, ps[i].y, colors[i]));
    }
    addElementBlock(GL_TRIANGLES, 3, 0.0f, m_BaseShdr->getId(), -1);
}

void getCornersOfRect(ANCHOR a, const Point2D& anch, double w, double h, double rot, Point2D corners[4]){
    Point2D *bl = corners, *tl = corners+1 , *br = corners+2, *tr = corners+3;

    switch(a){
        case ANCHOR_C:
            *bl = Point2D(anch.x - w/2.0f, anch.y - h/2.0f);
            *tl = Point2D(anch.x - w/2.0f, anch.y + h/2.0f);
            *br = Point2D(anch.x + w/2.0f, anch.y - h/2.0f);
            *tr = Point2D(anch.x + w/2.0f, anch.y + h/2.0f);
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

    bl->rotateAroundBy(anch, rot);
    tl->rotateAroundBy(anch, rot);
    br->rotateAroundBy(anch, rot);
    tr->rotateAroundBy(anch, rot);
}

void ImageBuilder::drawRect(ANCHOR a, const Point2D& anch, double w, double h, double rot, const Color& color){
    Point2D corners[4];
    getCornersOfRect(a, anch, w, h, rot, corners);
    Point2D bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

    int blIdx = addVert(bl.x, bl.y, color); 
    int tlIdx = addVert(tl.x, tl.y, color); 
    int brIdx = addVert(br.x, br.y, color); 
    int trIdx = addVert(tr.x, tr.y, color); 
    
    PB(m_VertIdx, blIdx);
    PB(m_VertIdx, tlIdx);
    PB(m_VertIdx, brIdx);
    PB(m_VertIdx, trIdx);
    PB(m_VertIdx, brIdx);
    PB(m_VertIdx, tlIdx);


    addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_BaseShdr->getId(), -1);
}

void ImageBuilder::drawQuad(const std::vector<Point2D>& pts, const Color& col){
    if(pts.size() < 4){
        std::cout << "WARNING::DRAW_QUAD Need at least 4 points but only " << pts.size() << " given, skipping..." << std::endl; 
        return;
    }
    drawPolygon(pts, col);
}

void ImageBuilder::drawGrid(const Point2D& top_left, double width, double height, Color* colors, int rows, int cols){
    // There are nrows of point, but that means n-1 "Lines" of triangle strips
    std::vector<std::vector<int>> gridVertIds;
    gridVertIds.resize(rows);
    for(int i = 0; i < rows; i++){
        gridVertIds[i].resize(cols);
        for(int j = 0; j < cols; j++){
            gridVertIds[i][j] = addVert(top_left.x + (1.0f*j*width)/cols, top_left.y + (1.0f*i*height)/rows, colors[i*cols + j]);
        }
    }

    int nElemsPre = m_VertIdx.size();
    for(int i = 0; i < rows-1; i++){
        m_VertIdx.push_back(gridVertIds[i][0]);
        for(int j = 0; j < cols-1; j++){
            m_VertIdx.push_back(gridVertIds[i+1][j]);
            m_VertIdx.push_back(gridVertIds[i][j+1]);
        }
        m_VertIdx.push_back(gridVertIds[i+1][cols-1]);
        // Duplicate that one, and first one of next loop
        // this will create "degenerate" triangles (all 3 vertices on the same line)
        // But at least we can draw all strips as a single element
        m_VertIdx.push_back(gridVertIds[i+1][cols-1]);
        m_VertIdx.push_back(gridVertIds[i+1][0]);
        
    }

    addElementBlock(GL_TRIANGLE_STRIP, m_VertIdx.size() - nElemsPre, 0.0f, m_BaseShdr->getId(), -1);

}

void ImageBuilder::drawPolygon(const std::vector<Point2D>&  pts, const Color& col){
    if(pts.size() < 3){
        std::cout << "WARNING::DRAW_POLYGON Need at least 3 points but only " << pts.size() << " given, skipping..." << std::endl; 
        return;
    }
    for(int i = 0; i < pts.size(); i++){
        m_VertIdx.push_back(addVert(pts[i].x, pts[i].y, col));
    }
    addElementBlock(GL_TRIANGLE_FAN, pts.size(), 0.0f, m_BaseShdr->getId(), -1);
}

// TODO better ways to draw cricle probably
void ImageBuilder::drawCircle(const Point2D& center, double r, const Color& color){
    drawEllipse(center, r, r, 0, color);
}

void ImageBuilder::drawEllipse(const Point2D& center, double rx, double ry, double rot, const Color& color){
    // Center point
    int startElemId = addVert(center.x, center.y, color);

    int precision = 180;
    // Circle points
    int last = -1, beforeLast = -1, first=-1;
    int px, py;
    Point2D edgePt;
    for(int i = 0; i < precision; i++){
        int px = center.x + cos(M_PI * i*2 / precision) * rx;
        int py = center.y + sin(M_PI * i*2 / precision) * ry;
        edgePt = {px, py};
        edgePt.rotateAroundBy(center, rot);
        beforeLast = last;
        last = addVert(edgePt.x, edgePt.y, color);
        if(i >= 1){
            PB(m_VertIdx, startElemId);
            PB(m_VertIdx, beforeLast);
            PB(m_VertIdx, last);
        } 
        if(i == 0){ first = last;}

    }
    PB(m_VertIdx, startElemId);
    PB(m_VertIdx, last);
    PB(m_VertIdx, first);
    addElementBlock(GL_TRIANGLES, precision*3, 0.0f, m_BaseShdr->getId(), -1);

}


void ImageBuilder::drawArc(const Point2D& center, double r, double from, double to, const Color& col){
    if(to < from){
        int tmp = from; from = to; to = tmp;
    }
    
    // Center point
    int startElemId = addVert(center.x, center.y, col);

    int tris = to-from;
    // Circle points
    int last = -1, beforeLast = -1;
    for(int i = from; i <= to; i++){
        int px = center.x + cos(M_PI * i*2/360) * r;
        int py = center.y + sin(M_PI * i*2/360) * r;
        beforeLast = last;
        last = addVert(px, py, col);
        if(i >= from+1){
            PB(m_VertIdx, startElemId);
            PB(m_VertIdx, beforeLast);
            PB(m_VertIdx, last);
        } 
    }
    addElementBlock(GL_TRIANGLES, tris*3, 0.0f, m_BaseShdr->getId(), -1);

}

void ImageBuilder::drawRing(const Point2D& center, double inR, double outR, const Color& color){
    drawRingArc(center, inR, outR, 0,360, color);
}

void ImageBuilder::drawRingArc(const Point2D& center, double inR, double outR, double from, double to, const Color& color){
    int precision = to-from;
    int outEID[precision], inEID[precision];

    // Ring points
    int inpx, inpy, outpx, outpy, arrIdx;
    // expensive computation
    double cosVal, sinVal;
    for(int i = from; i <= to; i++){
        arrIdx = i-from;
        cosVal = cos(M_PI * i*2 / 360);
        sinVal = sin(M_PI * i*2 / 360);

        inpx = center.x + cosVal * inR;
        inpy = center.y + sinVal * inR;
        outpx = center.x + cosVal * outR;
        outpy = center.y + sinVal * outR;

        inEID[arrIdx] = addVert(inpx, inpy, color);
        outEID[arrIdx] = addVert(outpx, outpy, color);

        if(i >= from+1){
            PB(m_VertIdx, inEID[arrIdx-1]);
            PB(m_VertIdx, outEID[arrIdx-1]);
            PB(m_VertIdx, outEID[arrIdx]);
            PB(m_VertIdx, inEID[arrIdx-1]);
            PB(m_VertIdx, inEID[arrIdx]);
            PB(m_VertIdx, outEID[arrIdx]);
        } 

    }
    
    addElementBlock(GL_TRIANGLES, precision*6, 0.0f, m_BaseShdr->getId(), -1);

}


void ImageBuilder::drawLine(const std::vector<Point2D>& ps, double w, const Color& col, bool loop){
    if(ps.size() == 1){
        std::cout << "ERROR::DRAW_LINE Only one point was given, not enough for a line " << std::endl; 
        return;
    }
    if(w > m_LineWidthRange[1]) {
        if(ps.size() == 2){
            // Requested line width is too high for GPU, simulate it with rectangle
            Point2D center = (ps[0] + ps[1])/2;
            Point2D orientation = ps[1] - ps[0];
            float angle = std::atan2(orientation.y, orientation.x);
            
            int length = std::sqrt(orientation.x*orientation.x + orientation.y*orientation.y);
            drawRect(ANCHOR_C, center, length, w, angle, col);
            return;
        }
        // Requested line width is too high for GPU
        // Since its a strip, cant really do rectangle
        std::cout << "ERROR::DRAW_LINE(n>2)::Width too big, max is " << m_LineWidthRange[1] << std::endl; 
        return;
    }
    
    // n > 2 
    // Pure OpenGL line
    for(int i = 0; i < ps.size(); i++){
        m_VertIdx.push_back(addVert(ps[i].x, ps[i].y, col));
    }
    addElementBlock(loop ? GL_LINE_LOOP : GL_LINE_STRIP, ps.size(), w, m_BaseShdr->getId(), -1);
}


void ImageBuilder::drawPoint(const Point2D& pt, double sz, const Color& color){
    m_VertIdx.push_back(addVert(pt.x, pt.y, color));
    addElementBlock(GL_POINTS, 1, sz, m_BaseShdr->getId(), -1);
}

void ImageBuilder::drawImage(ANCHOR a, const Point2D& anch, Texture* image, double w, double h, double rot){
    Point2D corners[4];
    getCornersOfRect(a, anch, w, h, rot, corners);
    Point2D bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

    int blIdx = addVert(bl.x, bl.y, 0.0f, 0.0f); 
    int tlIdx = addVert(tl.x, tl.y, 0.0f, 1.0f); 
    int brIdx = addVert(br.x, br.y, 1.0f, 0.0f); 
    int trIdx = addVert(tr.x, tr.y, 1.0f, 1.0f); 

    PB(m_VertIdx, blIdx);
    PB(m_VertIdx, tlIdx);
    PB(m_VertIdx, brIdx);
    PB(m_VertIdx, trIdx);
    PB(m_VertIdx, brIdx);
    PB(m_VertIdx, tlIdx);

    addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_TexShdr->getId(), image->getID());
}

void ImageBuilder::drawText(ANCHOR a, const Point2D& anch, const std::string& text, const Color& col, const char* fontName, double scale, double rot){
    // Great place to understand the following code: https://learnopengl.com/In-Practice/Text-Rendering

    // iterate through all characters
    std::string::const_iterator c;
    int x = anch.x, y = anch.y, blIdx, tlIdx, brIdx, trIdx;
    Point2D bl, tl, br, tr, nxtPos;
    Point2D corners[4];
    for (c = text.begin(); c != text.end(); c++)
    {
        Glyph* glyph = m_FontManager->getGlyph(*c, fontName);

        int xpos = x + (int)(glyph->bmL * scale);
        int ypos = y - (int)(glyph->h - glyph->bmT * scale);

        int w = (int)(glyph->w * scale);
        int h = (int)(glyph->h * scale);
        nxtPos = {xpos, ypos};
        getCornersOfRect(a, anch, w, h, rot, corners);
        bl = corners[0], tl = corners[1], br = corners[2], tr = corners[3];

        blIdx = addVert(bl.x, bl.y, col, 0.0f, 1.0f); 
        tlIdx = addVert(tl.x, tl.y, col, 0.0f, 0.0f); 
        brIdx = addVert(br.x, br.y, col, 1.0f, 1.0f); 
        trIdx = addVert(tr.x, tr.y, col, 1.0f, 0.0f); 

        PB(m_VertIdx, blIdx);
        PB(m_VertIdx, tlIdx);
        PB(m_VertIdx, brIdx);
        PB(m_VertIdx, trIdx);
        PB(m_VertIdx, brIdx);
        PB(m_VertIdx, tlIdx);


        addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f, m_GlyphShdr->getId(), glyph->tex->getID());
        
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (glyph->advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void ImageBuilder::drawBezier(const Point2D& p1, const Point2D& p2, const Point2D& c1, const Point2D& c2, const Color& col){
    int precision = 1000;
    int step = 40; // in per/thousand (precision)
    Point2D dir1 = c1 - p1;
    Point2D dir2 = c2 - c1;
    Point2D dir3 = p2 - c2;

    Point2D t1, t2, t3, dirt1, dirt2;
    Point2D f1, f2, dirf;
    int arrSize = precision; // Not all will be used
    std::vector<Point2D> pms(arrSize);
    int vertCount = 0;
    float i;
    for(int perc = 0; perc <= precision; perc+=step){
        i = 1.0f*perc/precision;
        t1 = p1 + dir1*i;
        t2 = c1 + dir2*i;
        t3 = c2 + dir3*i;
        dirt1 = t2 - t1;
        dirt2 = t3 - t2;
        f1 = t1 + dirt1*i;
        f2 = t2 + dirt2*i;
        dirf = f2-f1;
        pms[vertCount] = f1+dirf*i;
        vertCount++;
    }
    drawLine(pms, 1, col, false);
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
    glBufferData(GL_ARRAY_BUFFER, m_Verts.size()*sizeof(float), m_Verts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VertIdx.size()*sizeof(unsigned int), m_VertIdx.data(), GL_STATIC_DRAW);

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
    
    glClearColor( m_BgColor.r/255.0f, m_BgColor.g/255.0f, m_BgColor.b/255.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int currProgram = m_BaseShdr->getId();
    bool currWithTex = false;
    glUseProgram(currProgram);
    glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    int nBlock = m_ElemBlocks.size();
    //std::cout << "There are " << nBlock << " elem blocks" << std::endl;

    float viewMat[16];
    m_Camera->computeLookAtMatrix(viewMat);
    
    int viewMathLoc = glGetUniformLocation(currProgram, "viewMat");
    glUniformMatrix4fv(viewMathLoc, 1, GL_TRUE, viewMat);

    float camZoom = m_Camera->getZoom();
    int vertSpaceLoc = glGetUniformLocation(currProgram, "vertSpace");
    glUniform2f(vertSpaceLoc, m_Width/camZoom, m_Height/camZoom);

    for(int i = 0; i < nBlock; i++){
        ElementBlock& block = m_ElemBlocks[i];
        
        //std::cout << "Drawing block of mode " << block.mode << " from " << block.start << " of " << block.cnt << " elements, size " << block.size << " and texture " << block.texture << std::endl;

        if(block.shdrProg != currProgram){
            glUseProgram(block.shdrProg);
            currProgram = block.shdrProg;
            // Is this line necessary every time?
            viewMathLoc = glGetUniformLocation(currProgram, "viewMat");
            vertSpaceLoc = glGetUniformLocation(currProgram, "vertSpace");
            
            glUniformMatrix4fv(viewMathLoc, 1, GL_TRUE, viewMat);
            glUniform2f(vertSpaceLoc, m_Width/camZoom, m_Height/camZoom);

        }

        if(block.tex_id == -1){
            currWithTex = false;
        } else {
            currWithTex = true;
            glBindTexture(GL_TEXTURE_2D, block.tex_id);
        }

        if(block.mode == GL_LINES || block.mode == GL_LINE_STRIP || block.mode == GL_LINE_LOOP){
            glLineWidth(block.size*camZoom);
        } else if(block.mode == GL_POINTS){
            glPointSize(block.size*camZoom);
        }
        //glPointSize(10);
        glDrawElements(block.mode, block.cnt, GL_UNSIGNED_INT, (void*)(block.start*sizeof(unsigned int)));        
    }
    
    // TODO put this somewhere else, here prevents diff windows from having same src
    clearAll();

}



}
