#ifndef INC_3DGRAPHICSAPP_SHADER_H
#define INC_3DGRAPHICSAPP_SHADER_H

class Shader {
private:
    unsigned int shaderProgramId;

public:
    Shader(const char *vertexShaderCode, const char *fragmentShaderCode);
    void use() const;
    [[nodiscard]] int getUniformLocation(const char *uniformName) const;
    ~Shader();
};

#endif //INC_3DGRAPHICSAPP_SHADER_H