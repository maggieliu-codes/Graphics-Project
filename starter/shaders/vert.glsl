#version 330 core

// Vertex Attributes
layout(location = 0) in vec3 position;     // Position (attribute location 0)
layout(location = 1) in vec3 normal;       // Normal (attribute location 1)

// Outputs to Fragment Shader
out vec3 FragPos;
out vec3 Normal;

// Uniforms
uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    // Transform the vertex position to world space
    vec4 worldPosition = modelTransformMatrix * vec4(position, 1.0);
    FragPos = vec3(worldPosition);

    // Transform the normal to world space
    mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix)));
    Normal = normalize(normalMatrix * normal);

    // Calculate the final vertex position
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
