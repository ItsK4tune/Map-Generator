#include "util/config.h"
#include "util/input.h"
#include <iostream>

GLFWwindow *createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    if (width < 0 || height < 0)
    {
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

        window = glfwCreateWindow(mode->width, mode->height, "OpenGL renderWindow", primaryMonitor, NULL);
    }
    else
    {
        window = glfwCreateWindow(width, height, "OpenGL renderWindow", NULL, NULL);
    }

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    gladLoadGL();

    return window;
}

void configWindow(GLFWwindow *window)
{
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}