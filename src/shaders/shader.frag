#version 410 core
#define MAX_LIGHT_SOURCES 5
#define FOG_START 10
#define FOG_END 50

struct LightSource {
    vec3 position;
    vec3 color;
    int reflection;
    vec3 direction;
};

in vec3 vFragmentPos;
in vec3 vFragmentNormal;

uniform vec4 uObjectColor;
uniform int uLightSourceCount;
uniform LightSource uLightSources[MAX_LIGHT_SOURCES];
uniform int uIsDay;

out vec4 FragColor;

void main()
{
    float k_a = 0.01f;
    float k_d = 0.8f;
    float k_s = 1.2f;
    int alpha = 5;
    vec3 fogColor = vec3(0.0f, 0.0f, 0.0f);
    if (uIsDay > 0) {
        fogColor = vec3(0.5f, 0.6f, 0.7f);
    }
    vec3 N = normalize(vFragmentNormal);
    vec3 V = normalize(-vFragmentPos);
    vec3 objectColor = uObjectColor.xyz;
    vec3 sunDirection = vec3(0.0f, 1.0f, 0.0f);

    vec3 finalColor = k_a * objectColor;
    for (int i = 0; i < uLightSourceCount; i++) {
        vec3 L = normalize(uLightSources[i].position - vFragmentPos);
        vec3 R = 2 * dot(L, N) * N - L;

        vec3 tempColor = vec3(0.0f, 0.0f, 0.0f);
        tempColor += k_d * objectColor * uLightSources[i].color * max(dot(L, N), 0.0f);
        tempColor += k_s * objectColor * uLightSources[i].color * pow(clamp(dot(R, V), 0.0f, 1.0f), alpha);
        if (uLightSources[i].reflection > 0) {
            tempColor *= pow(clamp(dot(-L, normalize(uLightSources[i].direction)), 0.0f, 1.0f), float(uLightSources[i].reflection));
        }

        float dist = length(uLightSources[i].position - vFragmentPos);
        finalColor *= clamp(100.0f / dist / dist, 0.0f, 1.0f);

        finalColor += tempColor;
    }

    if (uIsDay > 0) {
        finalColor += k_d * objectColor * vec3(1.0f, 0.8f, 0.9f) * max(dot(sunDirection, N), 0.0f);
    }

    float dist = length(vFragmentPos);
    float fogFactor = (FOG_END - dist) / (FOG_END - FOG_START);
    fogFactor = clamp(fogFactor, 0.0f, 1.0f);

    FragColor = vec4(mix(fogColor, clamp(finalColor, 0.0f, 1.0f), fogFactor), uObjectColor.w);
}