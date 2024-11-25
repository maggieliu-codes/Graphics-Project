#version 330 core

// Outputs
out vec4 FragColor;

// Inputs from Vertex Shader
in vec2 v_texCoord;
in vec3 lightDirTangentSpace;
in vec3 viewDirTangentSpace;

// Uniforms
uniform sampler2D u_DiffuseMap; // Diffuse texture
uniform sampler2D u_NormalMap;  // Normal map texture

void main()
{
    // Sample the normal from the normal map (range [0,1])
    vec3 normal = texture(u_NormalMap, v_texCoord).rgb;

    // Transform normal to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);

    // Normalize light and view directions
    vec3 lightDir = normalize(lightDirTangentSpace);
    vec3 viewDir = normalize(viewDirTangentSpace);

    // Sample the diffuse color
    vec3 color = texture(u_DiffuseMap, v_texCoord).rgb;

    // Ambient component
    vec3 ambient = 0.1 * color;

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color;

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * vec3(0.5);

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
