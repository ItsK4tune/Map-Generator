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
#include <util/obj.h>
#include <util/light.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (globalCamera)
    {
        globalCamera->updateFromWindowSize(width, height);
    }
}

int main()
{
    GLFWwindow *window = createWindow(600, 600);
    configWindow(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Camera camera(glm::vec3(0.0f, 3.0f, -3.0f),
                  glm::vec3(0.0f, 3.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f),
                  45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    globalCamera = &camera;

    Object ground;
    generateTerrain(ground.getVertex(), 10);

    unsigned int groundVAO,
        groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);

    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, ground.getVertex().size() * sizeof(Vertex), ground.getVertex().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int groundProgram = createShaderProgram("shader/terrain/ground.vert", "shader/terrain/ground.frag");

    unsigned int modelLoc = glGetUniformLocation(groundProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(groundProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(groundProgram, "projection");

    Light sun(glm::vec3(0.0f, 5.0f, 0.0f),
              glm::vec3(1.0f, 1.0f, 1.0f),
              glm::vec3(0.1f),
              glm::vec3(0.8f),
              glm::vec3(1.0f));

    Material groundMaterial = {glm::vec3(0.1f, 0.1f, 0.1f),
                               glm::vec3(0.7f, 0.7f, 0.7f),
                               glm::vec3(1.0f, 1.0f, 1.0f),
                               32.0f};
    ground.setMaterial(groundMaterial);

    float cubeVertices[] = {
        // positions
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        -0.1f, 0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f,

        -0.1f, -0.1f, 0.1f,
        0.1f, -0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,
        -0.1f, -0.1f, 0.1f,

        -0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,

        0.1f, 0.1f, 0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,

        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, 0.1f,
        0.1f, -0.1f, 0.1f,
        -0.1f, -0.1f, 0.1f,
        -0.1f, -0.1f, -0.1f,

        -0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f};

    unsigned int sunVAO, sunVBO;
    glGenVertexArrays(1, &sunVAO);
    glGenBuffers(1, &sunVBO);

    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int sunShader = createShaderProgram("shader/terrain/sun.vert", "shader/terrain/sun.frag");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        glUseProgram(groundProgram);

        glUniform3fv(glGetUniformLocation(groundProgram, "lightPos"), 1, glm::value_ptr(sun.getPosition()));
        glUniform3fv(glGetUniformLocation(groundProgram, "lightColor"), 1, glm::value_ptr(sun.getColor()));
        glUniform3fv(glGetUniformLocation(groundProgram, "ambient"), 1, glm::value_ptr(sun.getAmbient()));
        glUniform3fv(glGetUniformLocation(groundProgram, "diffuse"), 1, glm::value_ptr(sun.getDiffuse()));
        glUniform3fv(glGetUniformLocation(groundProgram, "specular"), 1, glm::value_ptr(sun.getSpecular()));
        glUniform1f(glGetUniformLocation(groundProgram, "shininess"), ground.getShininess());

        glUniform3fv(glGetUniformLocation(groundProgram, "viewPos"), 1, glm::value_ptr(camera.getPosition()));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, ground.getVertex().size());

        glUseProgram(sunShader);

        glm::mat4 sunModel = glm::translate(glm::mat4(1.0f), sun.getPosition());
        glUniformMatrix4fv(glGetUniformLocation(sunShader, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));
        glUniformMatrix4fv(glGetUniformLocation(sunShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sunShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(sunShader, "color"), 1, glm::value_ptr(sun.getColor()));

        glBindVertexArray(sunVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glfwTerminate();

    return 0;
}
