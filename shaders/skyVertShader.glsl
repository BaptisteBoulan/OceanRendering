#version 410 core

layout (location=0) in vec3 vertPos;

uniform float fovy;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragDir;
out vec2 fragUV;

#define PI 3.14159265359

void main() {
    gl_Position = vec4(vertPos, 1.0f);
    fragUV = vertPos.xy;
    
    // Convertir de l'espace écran [-1,1] à un espace homogène avec le champ de vision
    float aspectRatio = projection[1][1] / projection[0][0];
    float fovScale = tan(fovy * 0.5);
    
    vec3 directionOffset = normalize(vec3(vertPos.x * aspectRatio * fovScale, vertPos.y * fovScale, 1.0));
    
    // Appliquer la matrice de vue pour obtenir la direction dans l'espace monde
    fragDir = normalize((view * vec4(directionOffset, 0.0)).xyz);
}
