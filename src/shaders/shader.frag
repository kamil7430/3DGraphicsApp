#version 410 core
#define MAX_LIGHT_SOURCES 5

struct LightSource {
    vec3 position;
    vec3 color;
};

in vec3 ourCameraPos;
in vec3 ourCameraNormal;

uniform vec4 uObjectColor;
uniform int uLightSourceCount;
uniform LightSource uLightSources[MAX_LIGHT_SOURCES];

out vec4 FragColor;

void main()
{
    float k_a = 0.2f;
    float k_d = 0.5f;
    float k_s = 0.5f;
    int alpha = 5;
    vec3 N = normalize(ourCameraNormal);
    vec3 V = normalize(-ourCameraPos);
    vec3 objectColor = uObjectColor.xyz;

    vec3 finalColor = k_a * objectColor;
    for (int i = 0; i < uLightSourceCount; i++) {
        vec3 L = normalize(uLightSources[i].position - ourCameraPos);
        vec3 R = 2 * dot(L, N) * N - L;

        finalColor += k_d * objectColor * uLightSources[i].color * max(dot(L, N), 0.0f);
        finalColor += k_s * objectColor * uLightSources[i].color * pow(clamp(dot(R, V), 0.0f, 1.0f), alpha);
    }
    FragColor = vec4(clamp(finalColor, 0.0f, 1.0f), 1.0f);
}