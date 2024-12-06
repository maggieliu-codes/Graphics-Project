// Geometry.hpp

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include "glm/vec3.hpp"

// Purpose of this class is to store vertex and triangle information
class Geometry
{
public:
	// Constructor
	Geometry();
	// Destructor
	~Geometry();

	// Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
	unsigned int GetBufferDataSize();
	float *GetBufferDataPtr();

	// Add a new vertex
	void AddVertex(float x, float y, float z, float nx, float ny, float nz);

	// Generates buffer data from stored vertices
	void Gen();

	// Functions for working with indices
	void MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2);
	unsigned int GetIndicesSize();
	unsigned int *GetIndicesDataPtr();

	// Adds an index (for OBJ loader)
	void AddIndex(unsigned int index);

private:
	// Vertex data components
	std::vector<float> m_vertexPositions;
	std::vector<float> m_normals;
	std::vector<float> m_textureCoords;

	// Combined buffer data
	std::vector<float> m_bufferData;

	// Indices for indexed drawing
	std::vector<unsigned int> m_indices;
};

#endif
