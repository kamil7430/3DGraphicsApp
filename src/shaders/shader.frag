#version 410 core
#define MAX_LIGHT_SOURCES 5

struct LightSource {
    vec3 position;
    vec3 color;
};

in vec3 ourCameraPos;
in vec3 ourCameraNormal;

out vec4 FragColor;

uniform vec4 uObjectColor;
uniform int uLightSourceCount;
uniform LightSource uLightSources[MAX_LIGHT_SOURCES];

void main()
{

}