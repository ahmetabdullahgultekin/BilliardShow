#include "Model3D.h"

Model3D::Model3D() {}

Model3D::~Model3D() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

bool Model3D::Load(const std::string &obj_model_filepath) {
    std::vector<glm::vec3> positions, normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> posIndices, normIndices, texIndices;

    std::ifstream file(obj_model_filepath);
    if (!file) {
        std::cerr << "Can't open OBJ: " << obj_model_filepath << "\n";
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        } else if (type == "vt") {
            glm::vec2 t;
            ss >> t.x >> t.y;
            texCoords.push_back(t);
        } else if (type == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (type == "f") {
            unsigned int pi[3], ti[3], ni[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> pi[i] >> slash >> ti[i] >> slash >> ni[i];
                posIndices.push_back(pi[i] - 1);
                texIndices.push_back(ti[i] - 1);
                normIndices.push_back(ni[i] - 1);
            }
        }
    }

    for (size_t i = 0; i < posIndices.size(); ++i) {
        Vertex vert;
        vert.position = positions[posIndices[i]];
        vert.normal = normals[normIndices[i]];
        vert.texCoord = texCoords[texIndices[i]];
        vertices.push_back(vert);
    }

    return true;
}

bool Model3D::Install() {
    std::cout << "Installing model..." << std::endl;
    glGenVertexArrays(1, &VAO);
    std::cout << "Model VAO created" << std::endl;
    glGenBuffers(1, &VBO);
    std::cout << "Model installed" << std::endl;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    std::cout << "Binding vertex attributes..." << std::endl;
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    // TexCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

    std::cout << "Vertex attributes bound" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Model3D::Render(const glm::vec3 &position) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    glBindVertexArray(0);

    glPopMatrix();
}