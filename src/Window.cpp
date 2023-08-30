#include "Window.hpp"

namespace Vizior {

/*void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    std::cout << "GLFW KEY CALLBACK GOT KEY " << key << std::endl;
    Window* viziorWin = (Window*) (glfwGetWindowUserPointer(window));
    viziorWin->keyCallback(key, scancode, action, mods);
}*/

void glfw_scroll_callback(GLFWwindow* window, double xscroll, double yscroll){
    std::cout << "GLFW SCROLL CALLBACK GOT YSCROLL " << yscroll << std::endl;
    Window* viziorWin = (Window*) (glfwGetWindowUserPointer(window));
    viziorWin->scrollCallback(xscroll, yscroll);
}

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

    m_Camera = new LinearCamera({m_Width/2, m_Height/2});
    glfwSetWindowUserPointer(m_glfw_Window, this);
    //glfwSetKeyCallback(m_glfw_Window, glfw_key_callback);
    glfwSetScrollCallback(m_glfw_Window, glfw_scroll_callback);
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
        
    m_Src->setCamera(m_Camera);
    m_Src->submit();

    glfwSwapBuffers(m_glfw_Window);

    /* Updates states for getkey */
    glfwPollEvents();
    updateCamPos();
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(m_glfw_Window);
}

void Window::setSource(std::shared_ptr<ImageBuilder> src){ 
    m_Src = src;
    src->setDimensions(m_Width, m_Height); 
}

void Window::updateCamPos()
{
    Point2D camDir{0,0};
    if (glfwGetKey(m_glfw_Window, GLFW_KEY_W) != GLFW_RELEASE)
        camDir.y+=1;
    if (glfwGetKey(m_glfw_Window, GLFW_KEY_S) != GLFW_RELEASE)
        camDir.y-=1;
    if (glfwGetKey(m_glfw_Window, GLFW_KEY_A) != GLFW_RELEASE)
        camDir.x-=1;
    if (glfwGetKey(m_glfw_Window, GLFW_KEY_D) != GLFW_RELEASE)
        camDir.x+=1;

    m_Camera->movePos(camDir);
    
}

void Window::scrollCallback(double x, double y)
{
    std::cout << "WINDOW SCROLL CALLBACK GOT VERT " << y << std::endl;
    
    // Delta zoom should be small when zoom already far from 1
    float zm = m_Camera->getZoom();

    m_Camera->setZoom(zm*(1+y/10.0f));
    
}

}