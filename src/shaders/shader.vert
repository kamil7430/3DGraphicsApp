#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 ourCameraPos;
out vec3 ourCameraNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    mat4 modelView = uView * uModel;

    vec4 cameraPos = modelView * vec4(aPos, 1.0f);
    ourCameraPos = cameraPos.xyz;

    ourCameraNormal = transpose(inverse(mat3(modelView))) * aNormal;

    gl_Position = uProjection * cameraPos;
}