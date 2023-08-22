#include "Window.hpp"

namespace Vizior {
    
Window::Window(int w, int h, const char* name)
    : m_WinName(name), m_Width(w), m_Height(h) {
    /* Initialize the library */
    if (!glfwInit()){
        std::cout << "Failed glfwInit(), make sure that external/glfw is populated" << std::endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    m_glfw_Window = glfwCreateWindow(m_Width, m_Height, name, NULL, NULL);
    if (!m_glfw_Window)
    {
        glfwTerminate();
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_glfw_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    } 

    glViewport(0,0,m_Width,m_Height);
    compileShaders();
}

void Window::compileShaders(){
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

Window::~Window(){
    glfwDestroyWindow(m_glfw_Window);
    if(m_Src != nullptr) m_Src.reset();
}

void Window::drawSource(){
    if(m_Src == nullptr){
        std::cout << "Trying to draw on window " << m_WinName << " without source" << std::endl;
        return;
    }
    std::cout << "Trying to draw on window " << m_WinName << std::endl;
        

    glGenVertexArrays(1,&m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    std::cout << "Src got " << m_Src->getVertCount() << " Vertices and " << m_Src->getElemCount() << " Elems" << std::endl;
    std::cout << "Src dims " << m_Src->getWidth() << " x " << m_Src->getHeight() << " px" << std::endl;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Src->getVertCount()*sizeof(float), m_Src->getVerts(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Src->getElemCount()*sizeof(unsigned int), m_Src->getEBO(), GL_STATIC_DRAW);

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
    
    
    
    glDrawElements(GL_TRIANGLES, m_Src->getElemCount(), GL_UNSIGNED_INT, 0);
    
    
    /* Swap front and back buffers */
    glfwSwapBuffers(m_glfw_Window);

    /* Poll for and process events */
    glfwPollEvents();

    m_Src->clearAll();

}

bool Window::shouldClose(){
    return glfwWindowShouldClose(m_glfw_Window);
}

void Window::setSource(std::shared_ptr<ImageBuilder> src){ 
    m_Src = src;
    src->setDimensions(m_Width, m_Height); 
}

}