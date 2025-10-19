#include "Shaders.hpp"

namespace vzr {

Shader::Shader(const char* vertCode, const char* fragCode){
    // ----------------------------------------------------
    //
    // Create and compile vertex shaders
    //
    // ----------------------------------------------------
    int success;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----------------------------------------------------
    //
    // Create and compile fragment shaders (color)
    //
    // ----------------------------------------------------
    

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int texFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

   
    // ----------------------------------------------------
    //
    // Build shader programs by linking shaders
    //
    // ----------------------------------------------------
    

    int shaderProgram = m_Id = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}

Shader::~Shader(){
    glUseProgram(0);
    glDeleteProgram(m_Id);
}

void Shader::use(){
    glUseProgram(m_Id);
}

}