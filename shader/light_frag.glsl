#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 TexCoords;

// Wordspace normal passed from vertex shader
in vec4 Normal;

in vec3 FragPos;

// The final color
out vec4 FragColor;

// light struct
struct PointLight {
    vec3 position;
    vec3 color;
};

#define NR_LIGHTS 2
uniform PointLight lights[NR_LIGHTS];

// material
uniform vec3 Ambient = vec3(0.05, 0.05, 0.05);
uniform vec3 Diffuse = vec3(0.8, 0.8, 0.8);
uniform vec3 Specular = vec3(1.0, 1.0, 1.0);

// camera
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5);
    
    // combine results
    vec2 tex = vec2(TexCoords.x, 1 - TexCoords.y);
    vec3 ambient = Ambient * vec3(texture(Texture, tex));
    vec3 diffuse = Diffuse * diff * vec3(texture(Texture, tex));
    vec3 specular = Specular * spec * vec3(texture(Texture, tex));

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (0.1 + 0.09 * distance + 0.03 * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.color;
}


void main() {
    vec3 norm = normalize(vec3(Normal));
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcPointLight(lights[0], norm, FragPos, viewDir);

    for(int i = 1; i < NR_LIGHTS; i++)
        result += CalcPointLight(lights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}