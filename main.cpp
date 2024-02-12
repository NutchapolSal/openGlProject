#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <vector>

#include "Libs/Mesh.h"
#include "Libs/Shader.h"
#include "Libs/Window.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";

// Fragment Shader
static const char *fShader = "Shaders/shader.frag";

void CreateTriangle() {
    GLfloat vertices[] =
        {
            -1.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f};
    GLfloat vertices2[] =
        {
            2.0f + -1.0f, -1.0f, 0.0f,
            2.0f + 0.0f, -1.0f, 1.0f,
            2.0f + 1.0f, -1.0f, 0.0f,
            2.0f + 0.0f, 1.0f, 0.0f};
    GLfloat vertices3[] =
        {
            -1.0f, 1.5f + -1.0f, 0.8f + 0.0f,
            0.0f, 1.5f + -1.0f, 0.8f + 1.0f,
            1.0f, 1.5f + -1.0f, 0.8f + 0.0f,
            0.0f, 1.5f + 1.0f, 0.8f + 0.0f};
    GLfloat vertices4[] =
        {
            3.0f + -1.0f, -1.0f, 0.0f,
            3.0f + 0.0f, -2.0f, 1.0f,
            3.0f + 1.0f, -0.5f, 0.7f,
            3.0f + 0.0f, 1.0f, 0.9f};

    unsigned int indices[] =
        {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2};

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices2, indices, 12, 12);
    meshList.push_back(obj2);
    Mesh *obj3 = new Mesh();
    obj3->CreateMesh(vertices3, indices, 12, 12);
    meshList.push_back(obj3);
    Mesh *obj4 = new Mesh();
    obj4->CreateMesh(vertices4, indices, 12, 12);
    meshList.push_back(obj4);
}

void CreateShaders() {
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main() {
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateTriangle();
    CreateShaders();

    // for secret room 3 enter - https://forms.gle/U9VE4pkYAPNvUW1H9

    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f);
    // Loop until window closed
    while (!mainWindow.getShouldClose()) {
        // Get + Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 4; i++) {
            GLuint uniformModel = 0, uniformProjection = 0;

            shaderList[i].UseShader();
            uniformModel = shaderList[i].GetUniformLocation("model");
            uniformProjection = shaderList[i].GetUniformLocation("projection");

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.3f, 0.0f, -2.5f));
            model = glm::rotate(model, 90.0f * 3.1415f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        // end draw

        // magic word - SAKURA

        mainWindow.swapBuffers();
    }

    return 0;
}
