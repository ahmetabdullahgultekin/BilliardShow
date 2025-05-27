/**
    * Basic Vertex Shader
    * This shader is used for rendering basic objects with lighting and texture mapping.
    * It takes vertex positions, normals, and texture coordinates as input.
    * The shader applies transformations and computes lighting based on the provided uniforms.
    * @file basic.vert
    * @version 1.0
    * @date 2025-05-27
    * @author Ahmet Abdullah Gultekin
*/
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

