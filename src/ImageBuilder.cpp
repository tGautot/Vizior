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

    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, m_LineWidthRange);
    std::cout << "Max pure ogl line width is " << m_LineWidthRange[1] << std::endl;
    // Will be set for real once bound to a window, polly not needed
    m_Width = 100;
    m_Height = 100;
    compileShaders();
}

ImageBuilder::~ImageBuilder(){
    delete[] m_Verts;
    delete[] m_VertIdx;
    delete[] m_ElemBlocks;
}

int ImageBuilder::addVert(int x, int y, Color& color){
    m_Verts[m_NextVertPos] = (2.0f * x/m_Width) -1;
    m_Verts[m_NextVertPos+1] = (2.0f * y/m_Height) -1;
    m_Verts[m_NextVertPos+2] = color.r/255.0;
    m_Verts[m_NextVertPos+3] = color.g/255.0;
    m_Verts[m_NextVertPos+4] = color.b/255.0;
    m_Verts[m_NextVertPos+5] = color.a/255.0;
    m_NextVertPos += m_nVertexVals;
    return (m_NextVertPos - m_nVertexVals)/m_nVertexVals;
}

void ImageBuilder::addElementBlock(GLenum mode, int elemCount, float size){
    if(size < 1.0f) size = 1.0f;
    if(m_NextElemBlockPos == 0){
        m_ElemBlocks[0] = {mode, 0, elemCount, size};
        m_NextElemBlockPos++;
        return;
    }
    ElementBlock lastBlock = m_ElemBlocks[m_NextElemBlockPos-1];
    GLenum lastMode = lastBlock.mode;
    if(lastMode == mode && (lastMode == GL_TRIANGLES || lastMode == GL_POINTS) && lastBlock.size == size){
        m_ElemBlocks[m_NextElemBlockPos-1].cnt += elemCount;
        return;
    }
    m_ElemBlocks[m_NextElemBlockPos] = {mode, lastBlock.start + lastBlock.cnt, elemCount, size};
    m_NextElemBlockPos++;
}

void ImageBuilder::clearAll(){
    m_NextElemPos = 0;
    m_NextVertPos = 0;
    m_NextElemBlockPos = 0;
}

void ImageBuilder::drawTriangle(Point2D* ps, Color& color){
    for(int i = 0; i < 3; i++){
        m_VertIdx[m_NextElemPos] = addVert(ps[i].x, ps[i].y, color);
        m_NextElemPos++;
    }
    addElementBlock(GL_TRIANGLES, 3, 0.0f);
}

void ImageBuilder::drawRectangle(ANCHOR a, Point2D& anch, int w, int h, float rot, Color& color){
    Point2D bl, tl, br, tr;

    switch(a){
        case ANCHOR_C:
            bl = Point2D(anch.x - w/2, anch.y - h/2);
            tl = Point2D(anch.x - w/2, anch.y + h/2);
            br = Point2D(anch.x + w/2, anch.y - h/2);
            tr = Point2D(anch.x + w/2, anch.y + h/2);
            break;
        case ANCHOR_BL:
            bl = Point2D(anch.x  , anch.y  );
            tl = Point2D(anch.x  , anch.y+h);
            br = Point2D(anch.x+w, anch.y  );
            tr = Point2D(anch.x+w, anch.y+h);
            break;
        case ANCHOR_TL:
            bl = Point2D(anch.x  , anch.y-h);
            tl = Point2D(anch.x  , anch.y  );
            br = Point2D(anch.x+w, anch.y-h);
            tr = Point2D(anch.x+w, anch.y  );
            break;
        case ANCHOR_BR:
            bl = Point2D(anch.x-w, anch.y  );
            tl = Point2D(anch.x-w, anch.y+h);
            br = Point2D(anch.x  , anch.y  );
            tr = Point2D(anch.x  , anch.y+h);
            break;
        case ANCHOR_TR:
            bl = Point2D(anch.x-w, anch.y-h);
            tl = Point2D(anch.x-w, anch.y  );
            br = Point2D(anch.x  , anch.y-h);
            tr = Point2D(anch.x  , anch.y  );
            break;
    }

    Point2D pbl = bl.rotateAroundBy(anch, rot);
    Point2D ptl = tl.rotateAroundBy(anch, rot);
    Point2D pbr = br.rotateAroundBy(anch, rot);
    Point2D ptr = tr.rotateAroundBy(anch, rot);

    int blIdx = addVert(pbl.x, pbl.y, color); 
    int tlIdx = addVert(ptl.x, ptl.y, color); 
    int brIdx = addVert(pbr.x, pbr.y, color); 
    int trIdx = addVert(ptr.x, ptr.y, color); 

    m_VertIdx[m_NextElemPos] = blIdx;
    m_VertIdx[m_NextElemPos+1] = tlIdx;
    m_VertIdx[m_NextElemPos+2] = brIdx;
    m_NextElemPos+=3;

    m_VertIdx[m_NextElemPos] = trIdx;
    m_VertIdx[m_NextElemPos+1] = brIdx;
    m_VertIdx[m_NextElemPos+2] = tlIdx;
    m_NextElemPos+=3;


    addElementBlock(GL_TRIANGLE_STRIP, 6, 0.0f);
}

void ImageBuilder::drawCircle(Point2D& center, int r, Color& color){
    int startVertIdx = m_NextVertPos;
    // Center Point
    addVert(center.x, center.y, color);

    int precision = 180;
    // Circle points
    for(int i = 0; i < precision; i++){
        int px = center.x + cos(M_PI * i*2 / precision) * r;
        int py = center.y + sin(M_PI * i*2 / precision) * r;
        if(i >= 1){
            m_VertIdx[m_NextElemPos] = startVertIdx/m_nVertexVals;
            m_VertIdx[m_NextElemPos+1] = (m_NextVertPos-m_nVertexVals)/m_nVertexVals;
            m_VertIdx[m_NextElemPos+2] = m_NextVertPos/m_nVertexVals;
            m_NextElemPos += 3;
        } 
        addVert(px, py, color);

    }
    m_VertIdx[m_NextElemPos] = startVertIdx/m_nVertexVals;
    m_VertIdx[m_NextElemPos+1] = (m_NextVertPos-m_nVertexVals)/m_nVertexVals;
    m_VertIdx[m_NextElemPos+2] = startVertIdx/m_nVertexVals + 1;
    m_NextElemPos += 3;
    addElementBlock(GL_TRIANGLES, precision*3, 0.0f);

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
    m_VertIdx[m_NextElemPos] = addVert(ps[0].x, ps[0].y, color);
    m_NextElemPos++;
    m_VertIdx[m_NextElemPos] = addVert(ps[1].x, ps[1].y, color);
    m_NextElemPos++;
    addElementBlock(GL_LINE_STRIP, 2, 1.0f*w);
}

void ImageBuilder::setDimensions(int w, int h){
    m_Width = w;
    m_Height = h;
}

void ImageBuilder::compileShaders(){
    // ----------------------------------------------------
    //
    // Create and compile vertex shader
    //
    // ----------------------------------------------------


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &m_VertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----------------------------------------------------
    //
    // Create and compile fragment shader (color)
    //
    // ----------------------------------------------------
    

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &m_FragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----------------------------------------------------
    //
    // Build shader program by linking shaders
    //
    // ----------------------------------------------------
    

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);

    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(m_ShaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ImageBuilder::submit(){   

    glGenVertexArrays(1,&m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    std::cout << "Src got " << getVertCount() << " Vertices and " << getElemCount() << " Elems" << std::endl;
    std::cout << "Src dims " << getWidth() << " x " << getHeight() << " px" << std::endl;

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    
    int nBlock = getElemBlockCount();
    std::cout << "There are " << nBlock << " elem blocks" << std::endl;
    for(int i = 0; i < nBlock; i++){
        ElementBlock block = m_ElemBlocks[i];
        std::cout << "Drawing block of mode " << block.mode << " from " << block.start << " of " << block.cnt << " size " << block.size;
        if(block.mode == GL_LINES || block.mode == GL_LINE_STRIP || block.mode == GL_LINE_LOOP){
            glLineWidth(block.size);
        } else if(block.mode == GL_POINTS){
            glPointSize(block.size);
        }
        glDrawElements(block.mode, block.cnt, GL_UNSIGNED_INT, (void*)(block.start*sizeof(unsigned int)));
        std::cout << " Error?: " << errno << std::endl;
        
    }
    

    clearAll();

}



}