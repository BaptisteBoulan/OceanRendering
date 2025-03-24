#version 410 core

in vec2 fragUV;
in vec3 fragDir;

uniform float iTime;

out vec4 screenColor;











// => THE CODE FROM skyColor.glsl WILL BE INSERTED HERE AT LINE 20

void main() {
    if (sunIntensity >= 0.)
        lightColor = vec3(1.0, min(1.0, 0.6 + 0.4 * sunIntensity), min(1.0, sunIntensity));
    else
        lightColor = vec3(1.,.6,0.) * exp(sunIntensity * 12.) ;

    screenColor = vec4 (skyColor(fragDir), 1.0f);
}