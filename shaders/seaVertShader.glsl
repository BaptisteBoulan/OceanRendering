#version 410 core

layout (location=0) in vec3 vertexPos;

out vec3 normal;
out vec2 fragUV;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float iTime;
uniform vec3 cameraPos;

#define PI 3.14159265359

// --- SEA ---

// Gravity constant (approximate)
const float g = 9.81;

// Generate a single trochoidal wave based on wavelength and amplitude
vec2 generateWave(vec2 p, float t, float wavelength, float amplitude) {
    float randomParam = fract(sin(dot(vec2(amplitude, 0.1545), vec2(1.123, 2.345))) * 43758.5453123);
    float u = p.y * randomParam + p.x * (1. - randomParam);
    float k = 2.0 * PI / wavelength;
    float omega = sqrt(9.81 * k);
    float phase = k * u - omega * t;
    return vec2(amplitude * sin(phase), amplitude * cos(phase));
}

// Compute sea surface height with LOD-based transition from geometry to shading
float seaLevel(vec2 p, float t, float viewDistance) {
    float h = 0.0;
    int N = 8; // Number of wave components
    float wavelength = 15.0;
    float amplitude = 0.5;
    float minLod = 2.0; // Min wave detail level before switching to normal-based shading
    float maxLod = 5.0; // Max wave detail before BRDF shading

    for (int i = 0; i < N; i++) {
        if (viewDistance > maxLod * wavelength + 10.) break; // Stop processing waves at large distances
        
        vec2 displacement = generateWave(p, t, wavelength, amplitude);

        if (viewDistance < minLod * wavelength + 10.) {
            // Use full displacement for near waves
            p.x += displacement.x;
            p.y += displacement.x;
            h += displacement.y;
        } else {
            // Store only normal perturbation for distant waves
            h += displacement.y * smoothstep(minLod * wavelength, maxLod * wavelength, viewDistance);
        }

        wavelength /= 1.8;
        amplitude /= 2.5;
    }
    
    return h;
}

// Computes the normal of the wave surface using finite differences
vec3 calcNormal(vec3 p, float t, float viewDistance) {
    float eps = 0.01;
    vec3 dx = vec3(eps, seaLevel(p.xz + vec2(eps, 0), t, viewDistance) - seaLevel(p.xz, t, viewDistance), 0);
    vec3 dz = vec3(0, seaLevel(p.xz + vec2(0, eps), t, viewDistance) - seaLevel(p.xz, t, viewDistance), eps);
    return normalize(cross(dx, dz));
}

void main() {
    float viewDistance = length(cameraPos - vertexPos);
    float h = seaLevel(vertexPos.xz, iTime, viewDistance);
    vec3 position = vec3(vertexPos.x, h, vertexPos.z);
    fragPos = position;
    gl_Position = projection * view * model *  vec4(position, 1.0);
    normal = calcNormal(position, iTime, viewDistance); 
    fragUV = gl_Position.xy;
}