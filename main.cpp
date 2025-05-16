#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

#include <util/loadShader.h>
#include <util/config.h>
#include <util/affine.h>
#include <util/camera.h>
#include <util/input.h>
#include <util/object.h>
#include <util/light.h>

int main()
{
    GLFWwindow *window = createWindow(-1, -1);
    configWindow(window);

    // Create ground
    Object ground;

    std::vector<Vertex> groundVert;
    generateTerrain(groundVert, 7);
    Mesh groundMesh(groundVert);
    ground.setMesh(groundMesh);

    Material groundMaterial = {glm::vec3(0.1f, 0.1f, 0.1f),
                               glm::vec3(0.7f, 0.7f, 0.7f),
                               glm::vec3(0.0f, 1.0f, 1.0f),
                               1000.0f};
    ground.setMaterial(groundMaterial);

    Shader groundShader("shader/terrain/ground.vert", "shader/terrain/ground.frag");
    ground.setShader(groundShader);

    Camera camera(glm::vec3(0.0f, 3.0f, -3.0f),
                  glm::vec3(0.0f, 3.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f),
                  45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    globalCamera = &camera;

    // Create sun
    Light sun(glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<Vertex> sunVert;
    createSunVertices(sunVert);
    Mesh sunMesh(sunVert);
    sun.setMesh(sunMesh);

    // Material sunMaterial = {glm::vec3(0.1f, 0.1f, 0.1f),
    //                            glm::vec3(0.7f, 0.7f, 0.7f),
    //                            glm::vec3(0.0f, 1.0f, 1.0f),
    //                            1000.0f};
    // sun.setMaterial(sunMaterial);

    Shader sunShader("shader/terrain/sun.vert", "shader/terrain/sun.frag");
    sun.setShader(sunShader);

    float time = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        time = glfwGetTime();
        sun.setPosition(glm::vec3(5 * cos(time), 5 * sin(time), 0));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 sunModel = glm::translate(glm::mat4(1.0f), sun.getPosition());

        ground.use();

        ground.getShader().setVec3("lightPos", sun.getPosition());
        ground.getShader().setVec3("lightColor", sun.getColor());

        ground.getShader().setVec3("objectPosition", ground.getPosition());
        ground.getShader().setVec3("ambient", ground.getAmbient());
        ground.getShader().setVec3("diffuse", ground.getDiffuse());
        ground.getShader().setVec3("specular", ground.getSpecular());
        ground.getShader().setFloat("shininess", ground.getShininess());

        ground.getShader().setVec3("viewPos", camera.getPosition());

        ground.getShader().setMat4("model", model);
        ground.getShader().setMat4("view", view);
        ground.getShader().setMat4("projection", projection);

        ground.draw();

        sun.use();

        sun.getShader().setMat4("model", sunModel);
        sun.getShader().setMat4("view", view);
        sun.getShader().setMat4("projection", projection);
        sun.getShader().setVec3("color", sun.getColor());

        sun.draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
