/**
    * @file ObjectLoader.cpp
    * @brief Implementation of ObjectLoader class for loading and rendering 3D models in OpenGL.
    * This file contains the implementation of the ObjectLoader class, which handles loading OBJ files,
    * parsing vertex data, and setting up OpenGL buffers for rendering.
    * @author Ahmet Abdullah Gultekin
    * @date 2025-05-27
    * @version 1.0
 */
#include "ObjectLoader.h"

struct Face {
    unsigned int v[3];
    unsigned int n[3];
    unsigned int t[3];
};

struct TempVertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex;
};

/**
 * @fn parseOBJ
 * @brief Parses an OBJ file and extracts vertex data.
 * This function reads an OBJ file, extracts vertex positions, normals, and texture coordinates,
 * and constructs a list of vertices for rendering.
 * @param path The path to the OBJ file.
 * @param outVertices Output vector to store the parsed vertices.
 * @param outMtlFile Output string to store the name of the material file if present.
 * @return True if parsing was successful, false otherwise.
 */
bool parseOBJ(const std::string &path, std::vector<ObjectLoader::Vertex> &outVertices, std::string &outMtlFile) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<Face> faces;
    std::ifstream file(path);
    if (!file.is_open()) {
        Logger::Error("Failed to open OBJ file: " + path);
        return false;
    }
    std::string line;
    outMtlFile.clear();
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "mtllib") {
            iss >> outMtlFile;
        } else if (type == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (type == "vn") {
            glm::vec3 n;
            iss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (type == "vt") {
            glm::vec2 t;
            iss >> t.x >> t.y;
            texcoords.push_back(t);
        } else if (type == "f") {
            Face face = {};
            for (int i = 0; i < 3; ++i) {
                std::string vert;
                iss >> vert;
                size_t p1 = vert.find('/');
                size_t p2 = vert.find('/', p1 + 1);
                face.v[i] = std::stoi(vert.substr(0, p1)) - 1;
                if (p1 != std::string::npos && p2 != std::string::npos && p2 > p1 + 1)
                    face.t[i] = std::stoi(vert.substr(p1 + 1, p2 - p1 - 1)) - 1;
                else
                    face.t[i] = 0;
                if (p2 != std::string::npos)
                    face.n[i] = std::stoi(vert.substr(p2 + 1)) - 1;
                else
                    face.n[i] = 0;
            }
            faces.push_back(face);
        }
    }
    outVertices.clear();
    for (const auto &face: faces) {
        for (int i = 0; i < 3; ++i) {
            ObjectLoader::Vertex v;
            v.position = positions[face.v[i]];
            v.normal = normals.empty() ? glm::vec3(0, 0, 1) : normals[face.n[i]];
            v.texCoord = texcoords.empty() ? glm::vec2(0, 0) : texcoords[face.t[i]];
            v.texCoord.y = 1.0f - v.texCoord.y; // Flip Y for OpenGL
            outVertices.push_back(v);
        }
    }
    return true;
}

/**
 * @brief Constructor for ObjectLoader.
 * Initializes the ObjectLoader with empty vertex data and OpenGL buffers.
 */
ObjectLoader::ObjectLoader() {}

/**
 * @brief Destructor for ObjectLoader.
 * Cleans up OpenGL buffers if they were created.
 */
ObjectLoader::~ObjectLoader() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

/**
 * @fn Load
 * @brief Loads an OBJ model from a file.
 * This function reads an OBJ file, extracts vertex data, and attempts to load a texture from the associated MTL file if present.
 * @param obj_model_filepath The path to the OBJ model file.
 * @return True if the model was loaded successfully, false otherwise.
 */
bool ObjectLoader::Load(const std::string &obj_model_filepath) {
    std::string mtlFile;
    bool ok = parseOBJ(obj_model_filepath, vertices, mtlFile);
    if (!ok) {
        Logger::Error("Failed to parse OBJ file: " + obj_model_filepath);
        return false;
    }
    // Try to load texture from .mtl if present
    if (!mtlFile.empty()) {
        // Find the directory of an obj file
        std::string dir = obj_model_filepath.substr(0, obj_model_filepath.find_last_of("/\\") + 1);
        std::ifstream mtl(dir + mtlFile);
        if (mtl.is_open()) {
            Logger::Info("Loaded MTL file: " + dir + mtlFile);
            std::string line;
            while (std::getline(mtl, line)) {
                std::istringstream iss(line);
                std::string type;
                iss >> type;
                if (type == "map_Kd") {
                    std::string texFile;
                    iss >> texFile;
                    SetTexture(dir + texFile);
                    break;
                }
            }
        } else if (!mtlFile.empty()) {
            Logger::Warn("Could not open MTL file: " + dir + mtlFile);
        }
    }
    return true;
}

/**
 * @fn Install
 * @brief Sets up OpenGL buffers for rendering the loaded model.
 * This function creates a Vertex Array Object (VAO) and Vertex Buffer Object (VBO),
 * uploads the vertex data to the GPU, and configures the vertex attributes.
 * @return True if the installation was successful, false otherwise.
 */
bool ObjectLoader::Install() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ObjectLoader::Vertex), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectLoader::Vertex), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectLoader::Vertex),
                          (void *) offsetof(ObjectLoader::Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ObjectLoader::Vertex),
                          (void *) offsetof(ObjectLoader::Vertex, texCoord));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

/**
 * @fn SetTexture
 * @brief Loads a texture from a file and binds it for rendering.
 * This function attempts to load a texture from the specified file path.
 * It checks for OpenGL errors before and after loading the texture,
 * and logs the success or failure of the operation.
 * @param path
 */
void ObjectLoader::SetTexture(const std::string &path) {
    GLenum errBefore = glGetError();
    if (errBefore != GL_NO_ERROR) {
        Logger::Warn("OpenGL error before loading texture: " + std::to_string(errBefore));
    }
    if (!this->texture.LoadFromFile(path)) {
        Logger::Error("Failed to load texture: " + path);
    } else {
        Logger::Info("Texture loaded successfully: " + path);
    }
    if (!texture.IsValid()) {
        Logger::Error("Texture is not valid after loading: " + path);
    } else {
        Logger::Info("Texture is valid: " + path);
    }
    GLenum errAfter = glGetError();
    if (errAfter != GL_NO_ERROR) {
        Logger::Error("OpenGL error after SetTexture: " + std::to_string(errAfter));
    }
}

/**
 * @fn Render
 * @brief Renders the loaded model at a specified position with a default scale and rotation.
 * This function sets up the model matrix, binds the texture if available,
 * and draws the model using the currently active shader.
 * @param position The position to render the model at.
 * @param scale The scale factor for the model (default is 1.0).
 */
void ObjectLoader::Render(const glm::vec3 &position, float scale, const glm::mat4 &rotation) const {
    Shader *shader = Shader::GetActiveShader();
    if (!shader) {
        Logger::Error("No active shader in ObjectLoader::Render");
        return;
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * rotation;
    model = glm::scale(model, glm::vec3(scale));
    shader->setMat4("model", model);
    if (texture.IsValid()) {
        shader->setBool("useTexture", true);
        texture.Bind();
    } else {
        shader->setBool("useTexture", false);
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    glBindVertexArray(0);
}

