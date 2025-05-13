#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "util/camera.h"

extern Camera *globalCamera;
extern glm::mat4 model;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

#endif
