// Sun Direction
float dayNightCycle = iTime * 0.2f; // Controls day-night transition, adjust speed with 0.1
vec3 lightDir = normalize(vec3(cos(dayNightCycle), -sin(dayNightCycle), 0.0f));

// Sun Color (morning to midday to evening)
vec3 lightColor;
float sunIntensity = sin(dayNightCycle); // Smooth day/night transition (using sine)

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
    // Color ranges
    vec3 midnightColor = vec3(0., 0., 0.1);
    vec3 sunriseSunsetColor = vec3(0.6, 0.3, 0.0);
    vec3 middayColor = vec3(0.47, 0.71, 1.0);
    
    vec3 interpolatedColor;

    if (sunHeight < 0.0) {
        // When sunHeight is negative (nighttime), interpolate from midnight to sunrise/sunset color
        float t = smoothstep(-1., 0.0, sunHeight); // Transition from midnightColor to sunriseSunsetColor
        interpolatedColor = mix(midnightColor, sunriseSunsetColor, t);
    } else {
        // When sunHeight is positive (daytime), interpolate from sunrise/sunset color to midday color
        float t = smoothstep(0.0, 1., sunHeight); // Transition from sunriseSunsetColor to middayColor
        interpolatedColor = mix(sunriseSunsetColor, middayColor, t);
    }

    return interpolatedColor;
}


vec3 skyColor(vec3 p) {
    float distanceToSun = length(normalize(p) + lightDir);
    float invertedDistance = pow(1./ max(1., 10. * distanceToSun),2.);
    vec3 sunColor = vec3(0.6,0.5,0.);
    
    float sunHeight = min(2., max(-2., -lightDir.y)); // To have a 0 to 1 value
    

    return skyBaseColor(sunHeight) + max(0.,fbm(7. * p + vec3(0.2 * iTime, 0., 0.2 * iTime))) + invertedDistance * sunColor;
}