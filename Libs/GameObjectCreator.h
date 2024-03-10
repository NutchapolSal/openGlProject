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

GameObject *CreateGameObject(const char *objName, const char *vShaderPath, const char *fShaderPath, glm::mat4 modelTransform);

#endif
