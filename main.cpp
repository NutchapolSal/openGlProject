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

#include "Libs/GameObject.h"
#include "Libs/GameObjectCreator.h"
#include "Libs/Mesh.h"
#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/stb_image.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader *> shaderList;
std::vector<GameObject *> gameObjects;

float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(4.0f, 4.0f, 1.0f);

void CreateGameObjects() {
    unsigned int blankTexture;
    glGenTextures(1, &blankTexture);
    glBindTexture(GL_TEXTURE_2D, blankTexture);
    unsigned char blankTextureData[] = {31, 31, 31, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blankTextureData);

    unsigned int blankSpecularMap;
    glGenTextures(1, &blankSpecularMap);
    glBindTexture(GL_TEXTURE_2D, blankSpecularMap);
    unsigned char blankSpecularMapData[] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blankSpecularMapData);

    GameObject::SetBlankTexture(blankTexture);
    GameObject::SetBlankSpecularMap(blankSpecularMap);

    gameObjects.push_back(CreateGameObject(
        "Models/suzanne",
        "Shaders/shader.vert",
        "Shaders/shader.frag",
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f))));

    gameObjects.push_back(
        CreateGameObject(
            "Models/cube",
            "Shaders/shader.vert",
            "Shaders/shader.frag",
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f))));

    gameObjects.push_back(
        CreateGameObject(
            "Blends/axis test",
            "Shaders/shader.vert",
            "Shaders/shader.frag",
            glm::mat4(1.0f)));

    gameObjects.push_back(
        CreateGameObject(
            "Blends/triangle test",
            "Shaders/shader.vert",
            "Shaders/shader.frag",
            glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f))));

    gameObjects.push_back(
        CreateGameObject(
            "Blends/triangle seam test",
            "Shaders/shader.vert",
            "Shaders/shader.frag",
            glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f))));

    gameObjects.push_back(
        CreateGameObject(
            "Blends/coin",
            "Shaders/shader.vert",
            "Shaders/shader.frag",
            glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -3.0f, 0.0f))));

    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
    gameObjects.push_back(
        CreateGameObject(
            "Models/cube",
            "Shaders/lightShader.vert",
            "Shaders/lightShader.frag",
            lightModel));
}

void checkMouse() {
    double xpos;
    double ypos;
    glfwGetCursorPos(mainWindow.getWindow(), &xpos, &ypos);

    static float lastX = xpos;
    static float lastY = ypos;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (!glfwGetWindowAttrib(mainWindow.getWindow(), GLFW_FOCUSED)) {
        return;
    }

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

int main() {
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateGameObjects();

    // for secret room 3 enter - https://forms.gle/U9VE4pkYAPNvUW1H9

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraForwardFlat = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
    glm::vec3 cameraUp = glm::cross(cameraRight, cameraDirection);

    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f);

    float deltaTime;
    float lastFrame;
    // Loop until window closed
    while (!mainWindow.getShouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Get + Handle user input events
        glfwPollEvents();

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mainWindow.getWindow(), GL_TRUE);
        }

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_I) == GLFW_PRESS) {
            pitch += 125.0f * deltaTime;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_K) == GLFW_PRESS) {
            pitch -= 125.0f * deltaTime;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_J) == GLFW_PRESS) {
            yaw -= 125.0f * deltaTime;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_L) == GLFW_PRESS) {
            yaw += 125.0f * deltaTime;
        }

        checkMouse();

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));

        cameraDirection = glm::normalize(direction);
        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraForwardFlat = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos += cameraForwardFlat * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos -= cameraForwardFlat * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            cameraPos -= cameraRight * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            cameraPos += cameraRight * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            cameraPos += up * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cameraPos -= up * deltaTime * 5.0f;
        }
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
            yaw = -90.0f;
            pitch = 0.0f;
        }

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = 5.0f;
        lightPos.z = sin(glfwGetTime() / 2.0f);
        gameObjects[gameObjects.size() - 1]->SetModelTransform(glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f, 0.2f, 0.2f)));

        // draw here
        for (auto &gameObject : gameObjects) {
            gameObject->UseShader();
            glUniform3fv(gameObjects[0]->GetShaderUniformLocation("lightColor"), 1, (GLfloat *)&lightColor);
            glUniform3fv(gameObjects[0]->GetShaderUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
            glUniform3fv(gameObjects[0]->GetShaderUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);
            glUniformMatrix4fv(gameObjects[0]->GetShaderUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(gameObjects[0]->GetShaderUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
            gameObject->Render();
        }

        glUseProgram(0);
        // end draw

        // magic word - SAKURA

        mainWindow.swapBuffers();
    }

    return 0;
}
