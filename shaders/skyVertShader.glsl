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
    fragUV = vertPos.xy; // Coordonnées de texture en [-1,1]

    vec4 pos = projection * view * vec4(vertPos, 0.0);
    gl_Position = pos.xyww; // Garde z = w pour conserver la profondeur

    fragDir = vertPos;
}
