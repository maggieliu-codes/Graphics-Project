#include "Object.hpp"
#include "Error.hpp"
#include <fstream> // For std::ifstream
#include <sstream> // For std::istringstream

Object::Object()
{
}

Object::~Object()
{
}

// TODO: In the future it may be good to
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string fileName)
{
        // Load our actual textures
        m_textureDiffuse.LoadTexture(fileName);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind()
{
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        // Diffuse map is 0 by default, but it is good to set it explicitly
        m_textureDiffuse.Bind(0);
        // We need to set the texture slot explicitly for the normal map
        m_normalMap.Bind(1);
        // Select our appropriate shader
        m_shader.Bind();
}

void Object::Update(unsigned int screenWidth, unsigned int screenHeight)
{
        // Bind shaders
        m_shader.Bind();

        // Set uniform for object color
        m_shader.SetUniform3f("objectColor", 0.8f, 0.5f, 0.3f); // Set to any color you like

        // Projection matrix
        m_projectionMatrix = glm::perspective(glm::radians(45.0f),
                                              ((float)screenWidth) / ((float)screenHeight),
                                              0.1f, 100.0f);

        glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f), // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f), // Target position
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );

        // Set the uniforms in our current shader
        m_shader.SetUniformMatrix4fv("modelTransformMatrix", m_transform.GetTransformMatrix());
        m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);
        m_shader.SetUniformMatrix4fv("viewMatrix", &viewMatrix[0][0]);

        // Set light and view positions
        m_shader.SetUniform3f("lightPos", 0.0f, 5.0f, 5.0f);
        m_shader.SetUniform3f("viewPos", 0.0f, 0.0f, 5.0f);
}

// Render our geometry
void Object::Render()
{
        // Call our helper function to just bind everything
        Bind();
        // Render data
        glDrawElements(GL_TRIANGLES,
                       m_geometry.GetIndicesSize(), // The number of indices, not triangles.
                       GL_UNSIGNED_INT,             // Make sure the data type matches
                       nullptr);                    // Offset pointer to the data.
                                                    // nullptr because we are currently bound
}

// Returns the actual transform stored in our object
// which can then be modified
Transform &Object::GetTransform()
{
        return m_transform;
}

void Object::LoadOBJ(std::string objFilePath, std::string textureFilePath)
{
        // Open the OBJ file
        std::ifstream objFile(objFilePath);
        if (!objFile.is_open())
        {
                std::cerr << "Failed to open OBJ file: " << objFilePath << std::endl;
                return;
        }

        // Temporary storage
        std::vector<glm::vec3> positions; // Vertex positions
        std::vector<glm::vec3> normals;   // Normals
        std::vector<unsigned int> positionIndices, normalIndices;

        std::string line;
        while (std::getline(objFile, line))
        {
                std::istringstream ss(line);
                std::string prefix;
                ss >> prefix;

                if (prefix == "v")
                {
                        // Vertex position
                        glm::vec3 position;
                        ss >> position.x >> position.y >> position.z;
                        positions.push_back(position);
                }
                else if (prefix == "vn")
                {
                        // Normal vector
                        glm::vec3 normal;
                        ss >> normal.x >> normal.y >> normal.z;
                        normals.push_back(normal);
                }
                else if (prefix == "f")
                {
                        // Face indices
                        unsigned int posIndex[3], normIndex[3];

                        for (int i = 0; i < 3; ++i)
                        {
                                std::string vertexString;
                                ss >> vertexString;

                                // Replace '//' with ' '
                                size_t pos = vertexString.find("//");
                                if (pos != std::string::npos)
                                {
                                        vertexString.replace(pos, 2, " ");
                                }
                                else
                                {
                                        std::cerr << "Error: Unexpected face format in OBJ file." << std::endl;
                                        return;
                                }

                                std::istringstream vertexStream(vertexString);
                                vertexStream >> posIndex[i] >> normIndex[i];

                                positionIndices.push_back(posIndex[i]);
                                normalIndices.push_back(normIndex[i]);
                        }
                }
        }
        objFile.close();

        // Create the geometry
        // Note: OBJ indices start at 1, so we subtract 1 when accessing vectors
        for (size_t i = 0; i < positionIndices.size(); ++i)
        {
                unsigned int posIndex = positionIndices[i] - 1;
                unsigned int normIndex = normalIndices[i] - 1;

                // Error checking to prevent out-of-bounds access
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

        // Generate the geometry
        m_geometry.Gen();

        // Create vertex buffer layout
        m_vertexBufferLayout.CreatePositionNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                                              m_geometry.GetIndicesSize(),
                                                              m_geometry.GetBufferDataPtr(),
                                                              m_geometry.GetIndicesDataPtr());

        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        m_shader.CreateShader(vertexShader, fragmentShader);
}