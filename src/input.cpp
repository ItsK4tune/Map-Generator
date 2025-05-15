#include "util/input.h"
#include "util/camera.h"
#include "util/affine.h"

Camera *globalCamera = nullptr;

float pitch = 0.0f, yaw = -90.0f;
float distanceToObject = 3.0f;
glm::vec3 objectRotation = glm::vec3(0.0f);
glm::vec3 objectScale = glm::vec3(1.0f);
glm::mat4 model = glm::mat4(1.0f);
// bool rotating = false;
// double startX = 0.0, startY = 0.0;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (!globalCamera || action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    float angleStep = 0.03f;
    float scaleStep = 0.03f;
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
    case GLFW_KEY_UP:
        objectRotation.x -= angleStep;
        break;
    case GLFW_KEY_DOWN:
        objectRotation.x += angleStep;
        break;
    case GLFW_KEY_LEFT:
        objectRotation.y -= angleStep;
        break;
    case GLFW_KEY_RIGHT:
        objectRotation.y += angleStep;
        break;
    case GLFW_KEY_PAGE_UP:
        objectScale += glm::vec3(scaleStep);
        break;
    case GLFW_KEY_PAGE_DOWN:
        objectScale -= glm::vec3(scaleStep);
        objectScale = glm::max(objectScale, glm::vec3(0.1f));
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    default:
        break;
    }

    model = createAffineTransformMatrix(objectScale, objectRotation, glm::vec3(0.0f));
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (globalCamera)
    {
        globalCamera->updateFromWindowSize(width, height);
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX = 0.0, lastY = 0.0;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double dx = xpos - lastX;
    double dy = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    yaw += dx * sensitivity;
    pitch += dy * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    if (globalCamera)
    {
        glm::vec3 position = globalCamera->getTarget() - glm::normalize(direction) * distanceToObject;
        globalCamera->setPosition(position);
    }
}
