#include "vizior.hpp"


namespace vizior {

int16_t targetFrameRate=60;
Color RED = {255,0,0,255};
Color GRN = {0,255,0,255};
Color BLU = {0,0,255,255};
Color WHT = {255,255,255,255};
Color BLK = {0,0,0,1};
int32_t winW, winH;

GLFWwindow* window;
unsigned int shaderProgram, EBO, VAO, VBO;

// Each "vertex" is xyrgba
const int nVertexVals = 6; // number of vals in one "vertex"

// At most 10000 vertices
// TODO change this or handle it better
float toDrawVerts[nVertexVals*10000]; 
int nextVertIdx = 0;
unsigned int elemIdx[10000]; // Our EBO buffer
int nextElemIdx;

int addVert(int x, int y, Color* color){
    toDrawVerts[nextVertIdx] = (2.0f * x/winW) -1;
    toDrawVerts[nextVertIdx+1] = (2.0f * y/winH) -1;
    toDrawVerts[nextVertIdx+2] = color->r/255.0;
    toDrawVerts[nextVertIdx+3] = color->g/255.0;
    toDrawVerts[nextVertIdx+4] = color->b/255.0;
    toDrawVerts[nextVertIdx+5] = color->a/255.0;
    nextVertIdx += nVertexVals;
    return (nextVertIdx - nVertexVals)/nVertexVals;
}

/**
 * @brief Rotates the point "toRot" around the point "arount" by "by" degrees (NOT RADIANTS) counter-clockwise
 * 
 * @param toRot 
 * @param around 
 * @param by 
 * @return Point2D 
 */
Point2D rotateAroundBy(Point2D toRot, Point2D* around, int by){
    // Probably should put point2d in a cloass and overload everything
    Point2D diff = {toRot.x - around->x, toRot.y - around->y};
    float r = sqrt(diff.x * diff.x + diff.y * diff.y);
    float ang = atan2(diff.y, diff.x);
    ang += M_PI * by / 180;
    Point2D rotated = {
        (int) (cos(ang) * r) + around->x,
        (int) (sin(ang) * r) + around->y
    };
    return rotated;
}

int createWindow(int w, int h, const char* name){
    winW = w; winH = h;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(winW, winH, name, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport(0,0,winW,winH);
    return 0;
}

void init(){

    // ----------------------------------------------------
    //
    // Create and compile vertex shader
    //
    // ----------------------------------------------------

    const char* vertexShaderSrc = 
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec4 aColor;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{ vertexColor = aColor;"
        "gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);}\n\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
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
    

    const char* fragmentShaderSrc =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{ FragColor = vertexColor; }\n\0";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
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
    

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

loop_func loop;
void setLoopFunc(loop_func f){
    loop = f;
}

void Start(){
    int fc = 0;
    while(!glfwWindowShouldClose(window)){
        loop(fc++);
        usleep(1000000 * 1.0/targetFrameRate);
        nextElemIdx = 0;
        nextVertIdx = 0;
    }
}

void drawAll(){

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nextVertIdx*sizeof(float), toDrawVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nextElemIdx*sizeof(unsigned int), elemIdx, GL_STATIC_DRAW);

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


    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, nextElemIdx, GL_UNSIGNED_INT, 0);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();

        
    
}

void drawTriangle(Point2D* ps, Color* color){
    for(int i = 0; i < 3; i++){
        elemIdx[nextElemIdx] = nextVertIdx/6;
        addVert(ps[i].x, ps[i].y, color);
        nextElemIdx++;
    }
}

void drawRectangle(ANCHOR a, Point2D* anch, int w, int h, int r, Color* color){
    Point2D pbl = rotateAroundBy({anch->x - w/2, anch->y - h/2}, anch, r);
    Point2D ptl = rotateAroundBy({anch->x - w/2, anch->y + h/2}, anch, r);
    Point2D pbr = rotateAroundBy({anch->x + w/2, anch->y - h/2}, anch, r);
    Point2D ptr = rotateAroundBy({anch->x + w/2, anch->y + h/2}, anch, r);
    int bl = addVert(pbl.x, pbl.y, color); // BL
    int tl = addVert(ptl.x, ptl.y, color); // TL
    int br = addVert(pbr.x, pbr.y, color); // BR
    int tr = addVert(ptr.x, ptr.y, color); // TR
    elemIdx[nextElemIdx] = bl;
    elemIdx[nextElemIdx+1] = tl;
    elemIdx[nextElemIdx+2] = br;
    nextElemIdx+=3;
    elemIdx[nextElemIdx] = tr;
    elemIdx[nextElemIdx+1] = br;
    elemIdx[nextElemIdx+2] = tl;
    nextElemIdx+=3;
}

void drawCircle(ANCHOR a, Point2D* anch, int r, Color* color){
    int startVertIdx = nextVertIdx;
    // Center Point
    addVert(anch->x, anch->y, color);

    // Circle points
    for(int i = 0; i < 180; i++){
        int px = anch->x + cos(3.141592 * i*2 / 180) * r;
        int py = anch->y + sin(3.141592 * i*2 / 180) * r;
        if(i >= 1){
            elemIdx[nextElemIdx] = startVertIdx/6;
            elemIdx[nextElemIdx+1] = (nextVertIdx-nVertexVals)/6;
            elemIdx[nextElemIdx+2] = nextVertIdx/6;
            nextElemIdx += 3;
        } 
        addVert(px, py, color);

    }
    elemIdx[nextElemIdx] = startVertIdx/6;
    elemIdx[nextElemIdx+1] = (nextVertIdx-nVertexVals)/6;
    elemIdx[nextElemIdx+2] = startVertIdx/6 + 1;
    nextElemIdx += 3;


}

void drawLine(Point2D* ps, int w, Color* color){
    Point2D center = { (ps[0].x + ps[1].x) / 2, (ps[0].y + ps[1].y) /2};
    Point2D orientation = { (ps[1].x - ps[0].x), (ps[1].y - ps[0].y)};
    int angle = 180 * atan2(orientation.y, orientation.x) / M_PI;
    std::cout << "Line angle is " << angle << std::endl;
    int length = sqrt(orientation.x*orientation.x + orientation.y*orientation.y);
    drawRectangle(ANCHOR_C, &center, length, w, angle, color);
    
}

}