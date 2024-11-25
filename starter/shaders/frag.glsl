#version 330 core

// Outputs
out vec4 FragColor;

// Inputs from Vertex Shader
in vec3 FragPos;
in vec3 Normal;

// Uniforms
uniform vec3 objectColor;  // New uniform for object color
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Use a solid color for the object
    vec3 color = objectColor;

    // Ambient component
    vec3 ambient = 0.1 * color;

    // Diffuse component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;

    // Specular component
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * vec3(1.0);

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
