#include "VertexBufferLayout.hpp"
#include <iostream>

VertexBufferLayout::VertexBufferLayout()
{
}

VertexBufferLayout::~VertexBufferLayout()
{
    // Delete our buffers that we have previously allocated
    glDeleteBuffers(1, &m_vertexPositionBuffer);
    glDeleteBuffers(1, &m_indexBufferObject);
    glDeleteVertexArrays(1, &m_VAOId);
}

void VertexBufferLayout::Bind()
{
    // Bind to our vertex array
    glBindVertexArray(m_VAOId);
    // Bind to our vertex information
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    // Bind to the elements we are drawing
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}

// Note: Calling Unbind is rarely done; if you need
// to draw something else, then just bind to new buffer.
void VertexBufferLayout::Unbind()
{
    // Unbind the VAO
    glBindVertexArray(0);
    // Unbind the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBufferLayout::CreatePositionNormalBufferLayout(unsigned int vcount, unsigned int icount, float *vdata, unsigned int *idata)
{
    m_stride = 6 * sizeof(float); // 3 for position, 3 for normal

    static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat and float are not the same size on this architecture");

    // Generate and bind the Vertex Array Object
    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);

    // Generate and bind the Vertex Buffer Object
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    // Generate and bind the Element Buffer Object (Index Buffer)
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);

    // Enable and set vertex attribute pointers
    // Position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,            // Attribute index (location = 0 in shader)
                          3,            // Number of components per vertex attribute (x, y, z)
                          GL_FLOAT,     // Data type
                          GL_FALSE,     // Normalize?
                          m_stride,     // Stride (byte offset between consecutive attributes)
                          (void *)(0)); // Offset of the first component

    // Normal (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,                            // Attribute index (location = 1 in shader)
                          3,                            // Number of components per vertex attribute (nx, ny, nz)
                          GL_FLOAT,                     // Data type
                          GL_FALSE,                     // Normalize?
                          m_stride,                     // Stride
                          (void *)(3 * sizeof(float))); // Offset of the first component (after position)

    // Unbind the VAO for safety
    glBindVertexArray(0);
}
