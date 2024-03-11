#include "GameObjectCreator.h"
#include <unordered_map>
std::unordered_map<std::string, Mesh *> meshMap;
std::unordered_map<std::string, Shader *> shaderMap;
std::unordered_map<std::string, unsigned int> textureMap;
std::unordered_map<std::string, unsigned int> specularMapMap;
std::unordered_map<std::string, bool> noDiffuseMap;
std::unordered_map<std::string, bool> noSpecularMap;

static void loadTexturePrivate(std::string texturePath, unsigned int *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width;
    int height;
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        throw std::runtime_error("Failed to load texture");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

static void LoadDiffuseMap(GameObject *gameObject, std::string texturePath) {
    if (textureMap.count(texturePath)) {
        gameObject->SetTexture(textureMap[texturePath]);
    } else if (noDiffuseMap.count(texturePath)) {
    } else {
        try {
            unsigned int texture;
            loadTexturePrivate(texturePath, &texture);
            gameObject->SetTexture(texture);
            textureMap[texturePath] = texture;
        } catch (std::runtime_error &e) {
            noDiffuseMap[texturePath] = true;
            throw e;
        }
    }
}

static void LoadSpecularMap(GameObject *gameObject, std::string specularMapPath) {
    if (specularMapMap.count(specularMapPath)) {
        gameObject->SetSpecularMap(specularMapMap[specularMapPath]);
    } else if (noSpecularMap.count(specularMapPath)) {
    } else {
        try {
            unsigned int specularMap;
            loadTexturePrivate(specularMapPath, &specularMap);
            gameObject->SetSpecularMap(specularMap);
            specularMapMap[specularMapPath] = specularMap;
        } catch (std::runtime_error &e) {
            noSpecularMap[specularMapPath] = true;
            throw e;
        }
    }
}

GameObject *CreateGameObject(std::string objName, std::string vShaderPath, std::string fShaderPath, glm::mat4 modelTransform) {
    std::string objFilename = objName + ".obj";
    std::string diffuseMapFilename = objName + " diffuse.png";
    std::string specularMapFilename = objName + " specular.png";

    Mesh *mesh;
    if (meshMap.count(objName)) {
        mesh = meshMap[objName];
    } else {
        mesh = new Mesh();
        mesh->CreateMeshFromOBJ(objFilename.c_str());
        meshMap[objName] = mesh;
    }

    Shader *shader;
    if (shaderMap.count(vShaderPath)) {
        shader = shaderMap[vShaderPath];
    } else {
        shader = new Shader();
        shader->CreateFromFiles(vShaderPath.c_str(), fShaderPath.c_str());
        shaderMap[vShaderPath] = shader;
    }

    GameObject *gameObject = new GameObject(mesh, shader, modelTransform);
    try {
        LoadDiffuseMap(gameObject, diffuseMapFilename);
    } catch (std::runtime_error &e) {
        std::cout << "No Diffuse Map for " << objName << std::endl;
    }
    try {
        LoadSpecularMap(gameObject, specularMapFilename);
    } catch (std::runtime_error &e) {
        std::cout << "No Specular Map for " << objName << std::endl;
    }

    return gameObject;
}
