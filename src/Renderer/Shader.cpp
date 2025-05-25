#include "Shader.h"

Shader *Shader::activeShader = nullptr;

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

void Shader::use() {
    glUseProgram(programID);
    activeShader = this;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setInt(const std::string &name, int value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1i(loc, value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(loc, 1, &value[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform4fv(loc, 1, &value[0]);
}

void Shader::setBool(const std::string &name, bool value) const {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1i(loc, value ? 1 : 0);
}

Shader *Shader::GetActiveShader() {
    return activeShader;
}
