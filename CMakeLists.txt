cmake_minimum_required(VERSION 3.13)
project(MyOpenGLProject)
find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_DIR CACHE PATH "example: ../libs/glfw-3.3.9/include")
set(GLFW_LIBRARY CACHE FILEPATH "example: ../libs/glfw-3.3.9.bin.WIN64/lib-mingw-w64/libglfw3dll.a")

set(GLEW_INCLUDE_DIR CACHE PATH "example: ../libs/glew-2.1.0/include")
set(GLEW_LIBRARY CACHE FILEPATH "example: ../libs/glew-2.1.0/lib/Release/x64/glew32.lib")

set(GLM_INCLUDE_DIR CACHE PATH "example: ../libs/glm/")

set(MY_SOURCE_FILES
    main.cpp
    Libs/Mesh.cpp
    Libs/Shader.cpp
    Libs/Window.cpp
    Libs/stb_image.cpp
    Libs/GameObject.cpp
    Libs/GameObjectCreator.cpp
)

add_executable(MyOpenGLProject
    ${MY_SOURCE_FILES}
)
include_directories(
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)


target_link_libraries(MyOpenGLProject
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARY}
)