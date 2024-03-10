#ifndef GAMEOBJECT____H
#define GAMEOBJECT____H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

struct GameObject {
private:
    static unsigned int blankTexture;
    static unsigned int blankSpecularMap;
    Mesh *mesh;
    Shader *shader;
    bool hasTexture = false;
    unsigned int texture;
    bool hasSpecularMap = false;
    unsigned int specularMap;
    glm::mat4 modelTransform;

public:
    static void SetBlankTexture(unsigned int texture) { blankTexture = texture; };
    static void SetBlankSpecularMap(unsigned int specularMap) { blankSpecularMap = specularMap; };

    GameObject(Mesh *mesh, Shader *shader, glm::mat4 modelTransform) {
        this->mesh = mesh;
        this->shader = shader;
        this->modelTransform = modelTransform;
    }

    void SetTexture(unsigned int texture) {
        this->texture = texture;
        this->hasTexture = true;
    }

    void SetSpecularMap(unsigned int specularMap) {
        this->specularMap = specularMap;
        this->hasSpecularMap = true;
    }

    void SetModelTransform(glm::mat4 modelTransform) { this->modelTransform = modelTransform; }

    void UseShader() { shader->UseShader(); }

    GLuint GetShaderUniformLocation(const char *uniformName) { return shader->GetUniformLocation(uniformName); }

    void Render() {
        glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelTransform));

        glUniform1i(shader->GetUniformLocation("diffuseMap"), 0);
        glActiveTexture(GL_TEXTURE0);
        if (this->hasTexture) {
            glBindTexture(GL_TEXTURE_2D, this->texture);
        } else {
            glBindTexture(GL_TEXTURE_2D, blankTexture);
        }

        glUniform1i(shader->GetUniformLocation("specularMap"), 1);
        glActiveTexture(GL_TEXTURE1);
        if (this->hasSpecularMap) {
            glBindTexture(GL_TEXTURE_2D, this->specularMap);
        } else {
            glBindTexture(GL_TEXTURE_2D, blankSpecularMap);
        }
        mesh->RenderMesh();
    }
};

#endif