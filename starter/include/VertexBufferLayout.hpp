#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <glad/glad.h>

class VertexBufferLayout
{
public:
    // Generates a new buffer
    VertexBufferLayout();
    // Destroys all of our buffers.
    ~VertexBufferLayout();
    // Selects the buffer to bind
    void Bind();
    // Unbind our buffers
    void Unbind();

    // Creates a vertex and index buffer object
    // Format is: x,y,z, nx,ny,nz
    void CreatePositionNormalBufferLayout(unsigned int vcount, unsigned int icount, float *vdata, unsigned int *idata);

private:
    // Vertex Array Object
    GLuint m_VAOId;
    // Vertex Buffer
    GLuint m_vertexPositionBuffer;
    // Index Buffer Object
    GLuint m_indexBufferObject;
    // Stride of data (how do I get to the next vertex)
    unsigned int m_stride{0};
};

#endif
