// Object.hpp

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>
#include <vector>
#include <string>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Object
{
public:
    Object();
    ~Object();

    void LoadTexture(std::string fileName);

    // Update object with camera info
    void Update(unsigned int screenWidth, unsigned int screenHeight, const glm::mat4 &viewMatrix, const glm::vec3 &cameraPos);

    // Render the object
    void Render();
    // Access the transform
    Transform &GetTransform();

    // LoadOBJ now returns bool to indicate success/failure
    bool LoadOBJ(std::string objFilePath, std::string textureFilePath);

private:
    void Bind();

    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;

    Shader m_shader;
    VertexBufferLayout m_vertexBufferLayout;
    Texture m_textureDiffuse;
    Texture m_normalMap;
    Transform m_transform;
    glm::mat4 m_projectionMatrix;
    Geometry m_geometry;
};

#endif
