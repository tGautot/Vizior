#include "Window.hpp"

namespace Vizior {

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    std::cout << "GLFW KEY CALLBACK GOT KEY " << key << std::endl;
    Window* viziorWin = (Window*) (glfwGetWindowUserPointer(window));
    viziorWin->keyCallback(key, scancode, action, mods);
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
    glfwSetKeyCallback(m_glfw_Window, glfw_key_callback);
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
        
    m_Src->setCamera(m_Camera);
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

void Window::keyCallback(int key, int scancode, int action, int mods)
{
    std::cout << "WINDOW KEY CALLBACK GOT KEY " << key << std::endl;
    Point2D camDir{0,0};
    if (key == GLFW_KEY_W && action != GLFW_RELEASE)
        camDir.y+=1;
    if (key == GLFW_KEY_S && action != GLFW_RELEASE)
        camDir.y-=1;
    if (key == GLFW_KEY_A && action != GLFW_RELEASE)
        camDir.x-=1;
    if (key == GLFW_KEY_D && action != GLFW_RELEASE)
        camDir.x+=1;

    m_Camera->movePos(camDir);
    
}

}