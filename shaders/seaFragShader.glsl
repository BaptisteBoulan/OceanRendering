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



void main() {
    vec3 waterColor = vec3(0.1, 0.3, 0.5);

    if (sunIntensity >= 0.)
        lightColor = vec3(1.0, min(1.0, 0.6 + 0.4 * sunIntensity), min(1.0, sunIntensity));
    else
        lightColor = vec3(1.,.6,0.) * exp(sunIntensity * 12.) ;


    // Sunlight reflection
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(lightDir, normal);
    float mettalic = 0.8 * pow(max(dot(viewDir, reflectDir), 0.0), 16.0); // mettalic highlight
    float spec = 0.9 * pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Specular highlight
    float diff = 0.7 * max(dot(normal, lightDir), 0.0); // Diffuse light
    
    vec3 skyLight = 0.1 * skyColor(reflect(viewDir,normal));
    
    vec3 finalColor = 0.1 * waterColor + lightColor * ((mettalic + diff) * waterColor + spec) + skyLight;// Mix light with water
    
    screenColor = vec4(finalColor, 1.0f);
}