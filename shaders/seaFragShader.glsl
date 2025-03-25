#version 410 core

in vec2 fragUV;
in vec3 fragPos;
in vec3 normal;

out vec4 screenColor;

uniform vec2 iResolution; // Passé depuis le programme OpenGL
uniform float iTime;
uniform vec3 cameraPos;
uniform mat4 view;

#define PI 3.14159265359





// => THE CODE FROM skyColor.glsl WILL BE INSERTED HERE AT LINE 20



struct Material {
    vec3 baseColor;
    float metallic;
    float specular;
    float diffuse;
};

Material water = Material(vec3(0.1, 0.3, 0.5), 0.8, 1.2, 0.3);

void main() {
    initLights();
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 finalColor = 0.1 * water.baseColor;
    
    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 reflectDir = reflect(lights[i].direction, normal);
        float metallicEffect = water.metallic * pow(max(dot(viewDir, reflectDir), 0.0), 12.0);
        float specularEffect = water.specular * pow(max(dot(viewDir, reflectDir), 0.0), 256.0);
        float diffuseEffect = water.diffuse * max(dot(normal, lights[i].direction), 0.0);
        
        finalColor += lights[i].color * ((metallicEffect + diffuseEffect) * water.baseColor + specularEffect);
    }
    
    vec3 skyLight = 0.1 * skyColor(reflect(viewDir, normal));
    finalColor += skyLight;
    
    screenColor = vec4(finalColor, 1.0);
}