#ifndef GAMEOBJECTCREATOR____H
#define GAMEOBJECTCREATOR____H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"

GameObject *CreateGameObject(std::string objName, std::string vShaderPath, std::string fShaderPath, glm::mat4 modelTransform);

#endif
