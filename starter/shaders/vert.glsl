#version 330 core

// Vertex Attributes
layout(location = 0) in vec3 position;     // Position (attribute location 0)
layout(location = 1) in vec3 normal;       // Normal (attribute location 1)
layout(location = 2) in vec2 texCoord;     // Texture Coordinate (attribute location 2)
layout(location = 3) in vec3 tangent;      // Tangent (attribute location 3)
layout(location = 4) in vec3 bitangent;    // Bitangent (attribute location 4)

// Outputs to Fragment Shader
out vec2 v_texCoord;
out vec3 lightDirTangentSpace;
out vec3 viewDirTangentSpace;

// Uniforms
uniform mat4 modelTransformMatrix;   // Model transformation matrix
uniform mat4 viewMatrix;             // View matrix
uniform mat4 projectionMatrix;       // Projection matrix
uniform vec3 lightPos;               // Light position in world space
uniform vec3 viewPos;                // Camera position in world space

void main()
{
    // Transform the vertex position to world space
    vec4 worldPosition = modelTransformMatrix * vec4(position, 1.0);
    vec3 FragPos = vec3(worldPosition);

    // Transform normals, tangents, and bitangents to world space
    mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);

    // Construct the TBN matrix
    mat3 TBN = mat3(T, B, N);

    // Calculate vectors from the fragment position to the light and camera positions
    vec3 lightVector = lightPos - FragPos;
    vec3 viewVector = viewPos - FragPos;

    // Transform the light and view vectors to tangent space
    lightDirTangentSpace = normalize(TBN * lightVector);
    viewDirTangentSpace = normalize(TBN * viewVector);

    // Pass the texture coordinates to the fragment shader
    v_texCoord = texCoord;

    // Calculate the final vertex position
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
