#ifndef BILLIARDSHOW_SHADER_H
#define BILLIARDSHOW_SHADER_H

#include <glm/glm.hpp>
#include <gl/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Shader {
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    ~Shader();

    void use();

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void setInt(const std::string &name, int value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;

    void setBool(const std::string &name, bool value) const;

    GLuint ID() const { return programID; }

    static Shader *GetActiveShader();

private:
    GLuint programID;
    static Shader *activeShader;
};

#endif //BILLIARDSHOW_SHADER_H
