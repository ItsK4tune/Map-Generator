#include "util/input.h"
#include "util/camera.h"
#include "util/affine.h"

Camera *globalCamera = nullptr;

bool rotating = false;
double startX = 0.0, startY = 0.0;
float pitch = 0.0f, yaw = 0.0f;
glm::mat4 model = glm::mat4(1.0f);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (!globalCamera || action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    float speed = 1.0f;
    glm::vec3 forward = glm::normalize(globalCamera->getTarget() - globalCamera->getPosition());
    glm::vec3 right = glm::normalize(glm::cross(forward, globalCamera->getUp()));
    glm::vec3 up = globalCamera->getUp();

    switch (key)
    {
    case GLFW_KEY_W:
        globalCamera->move(forward, speed);
        break;
    case GLFW_KEY_S:
        globalCamera->move(-forward, speed);
        break;
    case GLFW_KEY_A:
        globalCamera->move(-right, speed);
        break;
    case GLFW_KEY_D:
        globalCamera->move(right, speed);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        globalCamera->move(up, speed);
        break;
    case GLFW_KEY_LEFT_CONTROL:
        globalCamera->move(-up, speed);
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    default:
        break;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            rotating = true;
            glfwGetCursorPos(window, &startX, &startY);
        }
        else if (action == GLFW_RELEASE)
        {
            rotating = false;
        }
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (rotating)
    {
        double dx = xpos - startX;
        double dy = ypos - startY;

        float sensitivity = 0.001f;
        yaw += dx * sensitivity;
        pitch += dy * sensitivity;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        startX = xpos;
        startY = ypos;

        model = createAffineTransformMatrix(glm::vec3(1.0f),
                                            glm::vec3(pitch, yaw, 0),
                                            glm::vec3(0.0f));
    }
}