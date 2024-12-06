// Geometry.cpp

#include "Geometry.hpp"
#include <assert.h>
#include <iostream>

// Constructor
Geometry::Geometry()
{
}

// Destructor
Geometry::~Geometry()
{
}

// Adds a vertex with position, normal, and texture coordinate
void Geometry::AddVertex(float x, float y, float z, float nx, float ny, float nz)
{
	// Positions
	m_vertexPositions.push_back(x);
	m_vertexPositions.push_back(y);
	m_vertexPositions.push_back(z);

	// Normals
	m_normals.push_back(nx);
	m_normals.push_back(ny);
	m_normals.push_back(nz);
}

// Retrieves a pointer to our buffer data
float *Geometry::GetBufferDataPtr()
{
	return m_bufferData.data();
}

// Retrieves the size of our buffer data (number of floats)
unsigned int Geometry::GetBufferDataSize()
{
	return m_bufferData.size();
}

// Retrieves the number of bytes of our buffer data
unsigned int Geometry::GetBufferSizeInBytes()
{
	return m_bufferData.size() * sizeof(float);
}

// Generates the combined buffer data from the stored vertex components
void Geometry::Gen()
{
	assert((m_vertexPositions.size() / 3) == (m_normals.size() / 3));

	for (size_t i = 0; i < m_vertexPositions.size() / 3; ++i)
	{
		// Positions
		m_bufferData.push_back(m_vertexPositions[i * 3 + 0]);
		m_bufferData.push_back(m_vertexPositions[i * 3 + 1]);
		m_bufferData.push_back(m_vertexPositions[i * 3 + 2]);

		// Normals
		m_bufferData.push_back(m_normals[i * 3 + 0]);
		m_bufferData.push_back(m_normals[i * 3 + 1]);
		m_bufferData.push_back(m_normals[i * 3 + 2]);
	}
}

// Adds a triangle by specifying three vertex indices
void Geometry::MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2)
{
	m_indices.push_back(vert0);
	m_indices.push_back(vert1);
	m_indices.push_back(vert2);
}

// Retrieves the number of indices
unsigned int Geometry::GetIndicesSize()
{
	return m_indices.size();
}

// Retrieves a pointer to the indices data
unsigned int *Geometry::GetIndicesDataPtr()
{
	return m_indices.data();
}

// Adds an index (used in OBJ loader)
void Geometry::AddIndex(unsigned int index)
{
	m_indices.push_back(index);
}
