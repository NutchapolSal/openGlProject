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
    Mesh *mesh;
    Shader *shader;
    unsigned int texture;
    glm::mat4 modelTransform;

public:
    GameObject(Mesh *mesh, Shader *shader, unsigned int texture, glm::mat4 modelTransform) {
        this->mesh = mesh;
        this->shader = shader;
        this->texture = texture;
        this->modelTransform = modelTransform;
    }

    void UseShader() { shader->UseShader(); }

    GLuint GetShaderUniformLocation(const char *uniformName) { return shader->GetUniformLocation(uniformName); }

    void Render() {
        glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelTransform));

        glBindTexture(GL_TEXTURE_2D, texture);
        mesh->RenderMesh();
    }
};

#endif