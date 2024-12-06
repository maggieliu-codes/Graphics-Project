// Object.cpp
#include "Object.hpp"
#include "Error.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Object::Object()
{
}

Object::~Object()
{
}

void Object::LoadTexture(std::string fileName)
{
        m_textureDiffuse.LoadTexture(fileName);
}

void Object::Bind()
{
        m_vertexBufferLayout.Bind();
        m_textureDiffuse.Bind(0);
        m_normalMap.Bind(1);
        m_shader.Bind();
}

void Object::Update(unsigned int screenWidth, unsigned int screenHeight, const glm::mat4 &viewMatrix, const glm::vec3 &cameraPos)
{
        m_shader.Bind();

        // Set object color
        m_shader.SetUniform3f("objectColor", 0.8f, 0.5f, 0.3f);

        // Projection matrix
        m_projectionMatrix = glm::perspective(glm::radians(45.0f),
                                              (float)screenWidth / (float)screenHeight,
                                              0.1f, 100.0f);

        m_shader.SetUniformMatrix4fv("modelTransformMatrix", m_transform.GetTransformMatrix());
        m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);
        m_shader.SetUniformMatrix4fv("viewMatrix", &viewMatrix[0][0]);

        m_shader.SetUniform3f("lightPos", 0.0f, 5.0f, 5.0f);
        m_shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
}

void Object::Render()
{
        Bind();
        glDrawElements(GL_TRIANGLES,
                       m_geometry.GetIndicesSize(),
                       GL_UNSIGNED_INT,
                       nullptr);
}

Transform &Object::GetTransform()
{
        return m_transform;
}

bool Object::LoadOBJ(std::string objFilePath, std::string textureFilePath)
{
        std::ifstream objFile(objFilePath);
        if (!objFile.is_open())
        {
                std::cerr << "Failed to open OBJ file: " << objFilePath << std::endl;
                return false;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> positionIndices, normalIndices;

        std::string line;
        while (std::getline(objFile, line))
        {
                std::istringstream ss(line);
                std::string prefix;
                ss >> prefix;

                if (prefix == "v")
                {
                        glm::vec3 position;
                        ss >> position.x >> position.y >> position.z;
                        positions.push_back(position);
                }
                else if (prefix == "vn")
                {
                        glm::vec3 normal;
                        ss >> normal.x >> normal.y >> normal.z;
                        normals.push_back(normal);
                }
                else if (prefix == "f")
                {
                        unsigned int posIndex[3], normIndex[3];
                        for (int i = 0; i < 3; ++i)
                        {
                                std::string vertexString;
                                ss >> vertexString;

                                size_t pos = vertexString.find("//");
                                if (pos == std::string::npos)
                                {
                                        std::cerr << "Error: Unexpected face format in OBJ file." << std::endl;
                                        return false;
                                }

                                vertexString.replace(pos, 2, " ");
                                std::istringstream vertexStream(vertexString);
                                vertexStream >> posIndex[i] >> normIndex[i];

                                positionIndices.push_back(posIndex[i]);
                                normalIndices.push_back(normIndex[i]);
                        }
                }
        }
        objFile.close();

        for (size_t i = 0; i < positionIndices.size(); ++i)
        {
                unsigned int posIndex = positionIndices[i] - 1;
                unsigned int normIndex = normalIndices[i] - 1;

                if (posIndex >= positions.size())
                {
                        std::cerr << "Position index out of bounds: " << posIndex << std::endl;
                        continue;
                }

                if (normIndex >= normals.size())
                {
                        std::cerr << "Normal index out of bounds: " << normIndex << std::endl;
                        continue;
                }

                glm::vec3 position = positions[posIndex];
                glm::vec3 normal = normals[normIndex];

                m_geometry.AddVertex(position.x, position.y, position.z,
                                     normal.x, normal.y, normal.z);
                m_geometry.AddIndex(i);
        }

        m_geometry.Gen();

        m_vertexBufferLayout.CreatePositionNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                                              m_geometry.GetIndicesSize(),
                                                              m_geometry.GetBufferDataPtr(),
                                                              m_geometry.GetIndicesDataPtr());

        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        m_shader.CreateShader(vertexShader, fragmentShader);

        return true; // Successfully loaded
}
