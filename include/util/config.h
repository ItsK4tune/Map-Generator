#ifndef CONFIG_H
#define CONFIG_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *createWindow(int width, int height);
void configWindow(GLFWwindow *window);

#endif