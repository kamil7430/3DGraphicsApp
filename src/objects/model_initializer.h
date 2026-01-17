#ifndef INC_3DGRAPHICSAPP_MODEL_INITIALIZER_H
#define INC_3DGRAPHICSAPP_MODEL_INITIALIZER_H

#include <format>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct SubMesh {
    unsigned int indexCount;
    unsigned int baseIndex;
    unsigned int materialIndex;
};

static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

static void processMesh(const aiMesh *mesh, std::vector<float> &vertices, std::vector<unsigned int> &indices,
    const glm::mat4 &transform, std::vector<SubMesh> &subMeshes, const float normalizationScalar) {
    const unsigned int vertexOffset = vertices.size() / 6;
    unsigned int indexStartOffset = indices.size();
    const glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);

        pos = transform * pos;

        vertices.push_back(pos.x / normalizationScalar);
        vertices.push_back(pos.y / normalizationScalar);
        vertices.push_back(pos.z / normalizationScalar);

        glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        norm = normalMatrix * norm;
        norm = glm::normalize(norm);

        vertices.push_back(norm.x);
        vertices.push_back(norm.y);
        vertices.push_back(norm.z);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.push_back(mesh->mFaces[i].mIndices[j] + vertexOffset);
        }
    }

    SubMesh subMesh;
    subMesh.indexCount = mesh->mNumFaces * 3;
    subMesh.baseIndex = indexStartOffset * sizeof(unsigned int);
    subMesh.materialIndex = mesh->mMaterialIndex;

    subMeshes.push_back(subMesh);
}

static void processNode(const aiNode *node, const aiScene *scene, std::vector<float> &vertices,
    std::vector<unsigned int> &indices, const glm::mat4 &parentTransform, std::vector<SubMesh> &subMeshes,
    const float normalizationScalar) {
    const glm::mat4 nodeTransform = aiMatrix4x4ToGlm(node->mTransformation);
    const glm::mat4 globalTransform = parentTransform * nodeTransform;

    // Process all meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, vertices, indices, globalTransform, subMeshes, normalizationScalar);
    }

    // Do the same for children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, vertices, indices, globalTransform, subMeshes, normalizationScalar);
    }
}

inline void initialize3dModel(std::vector<float> &vertices, std::vector<unsigned int> &indices, const char source[],
    unsigned int sourceLength, const char hint[], std::vector<glm::vec4> &materialColors, std::vector<SubMesh> &subMeshes,
    const float normalizationScalar) {
    Assimp::Importer importer;

    auto loadedScene = importer.ReadFileFromMemory(source, sourceLength,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace, hint);

    if (nullptr == loadedScene) {
        throw std::runtime_error(std::format("Sports car model loading error:\n{}\n", importer.GetErrorString()));
    }

    for (unsigned int i = 0; i < loadedScene->mNumMaterials; i++) {
        const aiMaterial* mat = loadedScene->mMaterials[i];
        aiColor4D color(1.0f, 1.0f, 1.0f, 1.0f);

        if (AI_SUCCESS == mat->Get(AI_MATKEY_BASE_COLOR, color) || AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
            materialColors.push_back(glm::vec4(color.r, color.g, color.b, color.a));
        else
            throw std::runtime_error("Invalid color!");
    }

    processNode(loadedScene->mRootNode, loadedScene, vertices, indices, glm::mat4(1.0f), subMeshes, normalizationScalar);
}

#endif //INC_3DGRAPHICSAPP_MODEL_INITIALIZER_H