/**
 * @file Shader.cpp
 * @brief Implementation of the Shader class for OpenGL rendering.
 * Handles shader compilation, linking, and uniform management.
 * This file includes methods for reading shader source files,
 * compiling shaders, and setting various uniform types.
 * This file is part of the BilliardShow project.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 */
#include "Shader.h"

// Static member to keep track of the currently active shader
Shader *Shader::activeShader = nullptr;

/**
 * @fn readFile
 * @brief Reads the contents of a shader file.
 * @param path The file path to the shader source.
 * @return The contents of the file as a string.
 * If the file cannot be opened, an error message is printed and an empty string is returned.
 */
static std::string readFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Shader file not found: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

/**
 * @fn compileShader
 * @brief Compiles a shader of the specified type from source code.
 * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
 * @param src The source code of the shader.
 * @param path The file path for error reporting.
 * @return The compiled shader ID.
 * If compilation fails, an error message is printed and the shader ID is returned.
 */
static GLuint compileShader(GLenum type, const std::string &src, const std::string &path) {
    GLuint shader = glCreateShader(type);
    const char *csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[2048];
        glGetShaderInfoLog(shader, 2048, nullptr, info);
        std::cerr << "Shader compile error in " << path << ":\n" << info << std::endl;
        std::cerr << "Shader source:\n" << src << std::endl;
    }
    return shader;
}

/**
 * @class Shader
 * @brief Represents an OpenGL shader program.
 * Handles shader compilation, linking, and uniform management.
 */
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vsrc = readFile(vertexPath);
    std::string fsrc = readFile(fragmentPath);
    if (vsrc.empty()) {
        std::cerr << "Vertex shader source is empty! Path: " << vertexPath << std::endl;
    }
    if (fsrc.empty()) {
        std::cerr << "Fragment shader source is empty! Path: " << fragmentPath << std::endl;
    }
    GLuint vshader = compileShader(GL_VERTEX_SHADER, vsrc, vertexPath);
    GLuint fshader = compileShader(GL_FRAGMENT_SHADER, fsrc, fragmentPath);
    programID = glCreateProgram();
    glAttachShader(programID, vshader);
    glAttachShader(programID, fshader);
    glLinkProgram(programID);
    glDeleteShader(vshader);
    glDeleteShader(fshader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

/**
 * @fn use
 * @brief Activates the shader program for rendering.
 * Sets this shader as the currently active shader.
 */
void Shader::use() {
    glUseProgram(programID);
    activeShader = this;
}

/**
 * @fn setMat4
 * @brief Sets a 4x4 matrix uniform in the shader.
 * @param name The name of the uniform variable.
 * @param mat The matrix to set.
 */
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

/**
 * @fn setInt
 * @brief Sets an integer uniform in the shader.
 * @param name The name of the uniform variable.
 * @param value The integer value to set.
 */
void Shader::setInt(const std::string &name, int value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1i(loc, value);
}

/**
 * @fn setVec3
 * @brief Sets a 3D vector uniform in the shader.
 * @param name The name of the uniform variable.
 * @param value The vector to set.
 */
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(loc, 1, &value[0]);
}

/**
 * @fn setBool
 * @brief Sets a boolean uniform in the shader.
 * @param name The name of the uniform variable.
 * @param value The boolean value to set.
 */
void Shader::setBool(const std::string &name, bool value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1i(loc, value ? 1 : 0);
}

/**
 * @fn setFloat
 * @brief Sets a float uniform in the shader.
 * @param name The name of the uniform variable.
 * @param value The float value to set.
 */
void Shader::setFloat(const std::string &name, float value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1f(loc, value);
}

/**
 * @fn GetActiveShader
 * @brief Returns the currently active shader.
 * @return Pointer to the active Shader instance.
 * If no shader is active, returns nullptr.
 */
Shader *Shader::GetActiveShader() {
    return activeShader;
}