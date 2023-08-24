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
    glfwWindowHint(GLFW_SAMPLES, 4);

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
}

Window::~Window(){
    glfwDestroyWindow(m_glfw_Window);
    if(m_Src != nullptr) m_Src.reset();
}

void Window::drawSource(){
    glfwMakeContextCurrent(m_glfw_Window);
    
    if(m_Src == nullptr){
        std::cout << "Trying to draw on window " << m_WinName << " without source" << std::endl;
        return;
    }
    std::cout << "Trying to draw on window " << m_WinName << std::endl;
        
    m_Src->submit();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_glfw_Window);

    /* Poll for and process events */
    glfwPollEvents();
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(m_glfw_Window);
}

void Window::setSource(std::shared_ptr<ImageBuilder> src){ 
    m_Src = src;
    src->setDimensions(m_Width, m_Height); 
}

}