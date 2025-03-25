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

// --- CONSTANTES PHYSIQUES ---
const float g = 9.81; // Gravité terrestre
const float windSpeed = 10.0; // Vitesse du vent en m/s

// --- SPECTRE PIERSON-MOSKOWITZ ---
// Modélise la distribution d'énergie des vagues en fonction de la fréquence
float PM_Spectrum(float k) {
    float alpha = 0.0081; // Facteur empirique 
    float k_peak = g / (windSpeed * windSpeed); // Nombre d'onde correspondant à la fréquence de pic
    float omega_peak = sqrt(g * k_peak);
    
    return alpha * g * g / pow(k, 4.0) * exp(-1.25 * pow(k_peak / k, 2.0));
}

// --- GÉNÉRATION DES VAGUES ---
vec3 computeWaveDisplacement(vec2 p, float t) {
    vec3 displacement = vec3(0.0);
    int N = 120 ; // Nombre de composantes de vagues
    float lambda_min = 0.1;
    float lambda_max = 50.0;
    
    for (int i = 0; i < N; i++) {
        float randomTheta = fract(sin(float(i) * 57.23) * 43758.5453) * 0.5 * PI; // Fonction pseudo-aléatoire pour donner un angle proche de 0, afin de simulier des vagues presque dans le même sens
        vec2 waveDir = vec2(cos(randomTheta), sin(randomTheta));

        float wavelength = lambda_min + (lambda_max - lambda_min) * pow(float(i) / float(N), 4.0);
        float k = 2.0 * PI / wavelength;
        float omega = sqrt(g * k);
        float amplitude = sqrt(PM_Spectrum(k)) * 1.0; // Ajustement empirique
        
        float phase = dot(waveDir, p) * k - omega * t;
        displacement += vec3(waveDir.x, 1.0, waveDir.y) * (amplitude * sin(phase));
    }

    return displacement / float(N);
}

// --- CALCUL DE LA HAUTEUR ---
float seaLevel(vec2 p, float t) {
    return computeWaveDisplacement(p, t).y;
}

// --- CALCUL DE LA NORMALE (Approximation par différences finies) ---
vec3 calcNormal(vec3 p, float t) {
    float eps = 0.1;
    vec3 dx = vec3(eps, seaLevel(p.xz + vec2(eps, 0), t) - seaLevel(p.xz, t), 0);
    vec3 dz = vec3(0, seaLevel(p.xz + vec2(0, eps), t) - seaLevel(p.xz, t), eps);
    return normalize(cross(dx, dz));
}

void main() {
    vec3 displacement = computeWaveDisplacement(vertexPos.xz, iTime);
    vec3 position = vertexPos + displacement;
    
    fragPos = position;
    normal = calcNormal(position, iTime);
    fragUV = position.xz;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
