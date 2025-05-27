/**
 * @file Shader.h
 * @brief Header file for the Shader class.
 * This class encapsulates OpenGL shader program functionality,
 * including loading, compiling, and setting uniforms.
 * It provides methods to set various types of uniforms
 * and manage the active shader state.
 * This file is part of the BilliardShow project.
 * @author Ahmet Abdullah Gultekin
 * @date 2025-05-27
 * @version 1.0
 */
#ifndef BILLIARDSHOW_SHADER_H
#define BILLIARDSHOW_SHADER_H

#include <glm/glm.hpp>
#include <gl/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

/**
 * @class Shader
 * @brief Represents an OpenGL shader program.
 * Handles shader compilation, linking, and uniform management.
 * This class provides methods to set various types of uniforms
 * and manage the active shader state.
 */
class Shader {
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    ~Shader();

    /**
     * @fn use
     * @brief Activates the shader program for rendering.
     * Sets this shader as the currently active shader.
     */
    void use();

    /**
     * @fn setMat4
     * @brief Sets a 4x4 matrix uniform in the shader.
     * @param name The name of the uniform variable.
     * @param mat The matrix to set.
     */
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    /**
     * @fn setInt
     * @brief Sets an integer uniform in the shader.
     * @param name The name of the uniform variable.
     * @param value The integer value to set.
     */
    void setInt(const std::string &name, int value) const;

    /**
     * @fn setVec3
     * @brief Sets a vec3 uniform in the shader.
     * @param name The name of the uniform variable.
     * @param value The vec3 value to set.
     */
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    /**
     * @fn setBool
     * @brief Sets a boolean uniform in the shader.
     * @param name The name of the uniform variable.
     * @param value The boolean value to set.
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * @fn setFloat
     * @brief Sets a float uniform in the shader.
     * @param name The name of the uniform variable.
     * @param value The float value to set.
     */
    void setFloat(const std::string &name, float value) const;

    /**
     * @fn ID
     * @brief Returns the OpenGL program ID of this shader.
     * @return The OpenGL program ID.
     */
    GLuint ID() const { return programID; }

    /**
     * @fn GetActiveShader
     * @brief Returns the currently active shader.
     * @return Pointer to the currently active Shader instance.
     */
    static Shader *GetActiveShader();

private:
    GLuint programID;
    static Shader *activeShader;
};

#endif //BILLIARDSHOW_SHADER_H
