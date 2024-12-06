#version 330 core

// Outputs
out vec4 FragColor;

// Inputs from Vertex Shader
in vec3 FragPos;
in vec3 Normal;

// Uniforms
uniform vec3 objectColor;  // Object color
uniform vec3 lightPos;     // Light position
uniform vec3 viewPos;      // Camera position

void main()
{
    // Use a solid color for the object
    vec3 color = objectColor;

    // Ambient component
    vec3 ambient = 0.1 * color;

    // Normalize input normals and calculate light direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse component
    float diff = max(dot(norm, lightDir), 0.0);

    // Toon Shading: Quantize the Diffuse Component
    float levels = 4.0; // Number of shading levels (adjust for effect)
    diff = floor(diff * levels) / levels;
    vec3 diffuse = diff * color;

    // Specular component
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

    // Quantize the Specular Component
    spec = floor(spec * levels) / levels;
    vec3 specular = specularStrength * spec * vec3(1.0);

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;

    // **Distance Fog Implementation**

    // Calculate the distance from the fragment to the camera
    float distance = length(viewPos - FragPos);

    // Fog parameters
    float fogStart = 5.0;    // Distance where fog starts
    float fogEnd = 20.0;     // Distance where fog is fully opaque
    vec3 fogColor = vec3(0.5, 0.5, 0.5); // Fog color (gray)

    // Compute the fog factor
    float fogFactor = clamp((distance - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

    // Blend the fragment color with the fog color
    vec3 finalColor = mix(result, fogColor, fogFactor);

    // Set the output color
    FragColor = vec4(finalColor, 1.0);
}
