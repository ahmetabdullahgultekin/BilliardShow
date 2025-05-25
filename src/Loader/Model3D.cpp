#include "Model3D.h"

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

bool parseOBJ(const std::string &path, std::vector<Model3D::Vertex> &outVertices, std::string &outMtlFile) {
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
            Model3D::Vertex v;
            v.position = positions[face.v[i]];
            v.normal = normals.empty() ? glm::vec3(0, 0, 1) : normals[face.n[i]];
            v.texCoord = texcoords.empty() ? glm::vec2(0, 0) : texcoords[face.t[i]];
            v.texCoord.y = 1.0f - v.texCoord.y; // Flip Y for OpenGL
            outVertices.push_back(v);
        }
    }
    return true;
}

Model3D::Model3D() {}

Model3D::~Model3D() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

bool Model3D::Load(const std::string &obj_model_filepath) {
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

bool Model3D::Install() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Model3D::Vertex), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex),
                          (void *) offsetof(Model3D::Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model3D::Vertex),
                          (void *) offsetof(Model3D::Vertex, texCoord));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}

void Model3D::SetTexture(const std::string &path) {
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
        // Only bind if valid, but usually binding is done in Render
        // this->texture.Bind();
    }
    GLenum errAfter = glGetError();
    if (errAfter != GL_NO_ERROR) {
        Logger::Error("OpenGL error after SetTexture: " + std::to_string(errAfter));
    }
}

void Model3D::Render(const glm::vec3 &position, float scale) const {
    Shader *shader = Shader::GetActiveShader();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
    shader->setMat4("model", model);
    shader->setBool("useTexture", texture.IsValid());
    shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Default color, can be changed

    if (texture.IsValid()) {
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        shader->setInt("texture1", 0);
        shader->setBool("useTexture", true);
    } else {
        shader->setBool("useTexture", false);
    }
    /*glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale, scale, scale);*/


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    glBindVertexArray(0);
    /*glPopMatrix();*/
    /*if (texture.IsValid()) glDisable(GL_TEXTURE_2D);*/
}