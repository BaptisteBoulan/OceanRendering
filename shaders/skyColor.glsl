#define NUM_LIGHTS 2

struct Light {
    vec3 direction;
    vec3 color;
};

Light lights[NUM_LIGHTS];
float sunIntensity;

void initLights() {
    float dayNightCycle = iTime * 0.1f;
    vec3 sunDir = normalize(vec3(0.0f, -sin(dayNightCycle), cos(dayNightCycle)));
    vec3 moonDir = normalize(vec3(sin(0.5f * dayNightCycle), - 0.5f, cos(0.5f * dayNightCycle)));
    sunIntensity = sin(dayNightCycle);
    vec3 sunColor = sunIntensity >= 0.0 ? vec3(1.0, min(1.0, 0.6 + 0.4 * sunIntensity), min(1.0, sunIntensity)) : vec3(1.0, 0.6, 0.0) * exp(sunIntensity * 12.0);
    
    lights[0] = Light(sunDir, sunColor);
    lights[1] = Light(moonDir, vec3(1.0) * 0.5);
}

vec3 hash(vec3 p) {
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
             dot(p, vec3(269.5, 183.3, 246.1)),
             dot(p, vec3(113.5, 271.9, 124.6)));
    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float perlinNoise3D(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
    vec3 u = f * f * (3.0 - 2.0 * f);
    
    float n000 = dot(hash(i + vec3(0,0,0)), f - vec3(0,0,0));
    float n100 = dot(hash(i + vec3(1,0,0)), f - vec3(1,0,0));
    float n010 = dot(hash(i + vec3(0,1,0)), f - vec3(0,1,0));
    float n110 = dot(hash(i + vec3(1,1,0)), f - vec3(1,1,0));
    float n001 = dot(hash(i + vec3(0,0,1)), f - vec3(0,0,1));
    float n101 = dot(hash(i + vec3(1,0,1)), f - vec3(1,0,1));
    float n011 = dot(hash(i + vec3(0,1,1)), f - vec3(0,1,1));
    float n111 = dot(hash(i + vec3(1,1,1)), f - vec3(1,1,1));

    return mix(mix(mix(n000, n100, u.x), mix(n010, n110, u.x), u.y),
               mix(mix(n001, n101, u.x), mix(n011, n111, u.x), u.y), u.z);
}

float fbm(vec3 p) {
    float f = 0.0;
    float a = 0.6;
    for (int i = 0; i < 5; i++) {
        f += a * perlinNoise3D(p);
        p *= 2.;
        a *= 0.5;
    }
    return f;
}

vec3 skyBaseColor(float sunHeight) {
    vec3 midnightColor = vec3(0., 0., 0.1);
    vec3 sunriseSunsetColor = vec3(0.6, 0.3, 0.0);
    vec3 middayColor = vec3(0.47, 0.71, 1.0);
    
    vec3 interpolatedColor;
    if (sunHeight < 0.0) {
        float t = smoothstep(-1., 0.0, sunHeight);
        interpolatedColor = mix(midnightColor, sunriseSunsetColor, t);
    } else {
        float t = smoothstep(0.0, 1., sunHeight);
        interpolatedColor = mix(sunriseSunsetColor, middayColor, t);
    }
    return interpolatedColor;
}

vec3 skyColor(vec3 p) {
    initLights();
    float distanceToSun = length(normalize(p) + lights[0].direction);
    float invertedDistanceToSun = pow(1. / max(1., 10. * distanceToSun), 2.);

    float distanceToMoon = length(normalize(p) + lights[1].direction);
    float invertedDistanceToMoon = pow(1. / max(1., 25. * distanceToMoon), 2.);

    vec3 sunColor = vec3(0.6, 0.5, 0.0);
    return skyBaseColor(sunIntensity) + max(0., fbm(7. * p + vec3(0.2 * iTime, 0., 0.2 * iTime))) + invertedDistanceToSun * sunColor + invertedDistanceToMoon * vec3(0.5f);
}
