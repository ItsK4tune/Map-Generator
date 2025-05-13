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

#define terrainSize 100
#define terrainHalfSize (terrainSize / 2)

struct vertex
{
    float pos[3];
    float uv[2];
    float color[3];
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (globalCamera)
    {
        globalCamera->updateFromWindowSize(width, height);
    }
}

void normalRandom(std::vector<vertex> &vertices, std::vector<unsigned int> &indices)
{
    for (int x = 0; x < terrainSize - 1; ++x)
    {
        for (int y = 0; y < terrainSize - 1; ++y)
        {
            float z1 = static_cast<float>(rand()) / RAND_MAX;
            float z2 = static_cast<float>(rand()) / RAND_MAX;
            float z3 = static_cast<float>(rand()) / RAND_MAX;
            float z4 = static_cast<float>(rand()) / RAND_MAX;

            float color1[3] = {z1, 0.0f, 1.0f - z1};
            float color2[3] = {z2, 0.0f, 1.0f - z2};
            float color3[3] = {z3, 0.0f, 1.0f - z3};
            float color4[3] = {z4, 0.0f, 1.0f - z4};

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z1, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color1[0], color1[1], color1[2]}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color2[0], color2[1], color2[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z3, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color3[0], color3[1], color3[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z3, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color3[0], color3[1], color3[2]}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color2[0], color2[1], color2[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z4, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color4[0], color4[1], color4[2]}});
        }
    }
}

void normalRandomWithFixedZ(std::vector<vertex> &vertices)
{
    std::vector<std::vector<float>> heightMap(terrainSize, std::vector<float>(terrainSize));
    for (int x = 0; x < terrainSize; ++x)
    {
        for (int y = 0; y < terrainSize; ++y)
        {
            heightMap[x][y] = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    for (int x = 0; x < terrainSize - 1; ++x)
    {
        for (int y = 0; y < terrainSize - 1; ++y)
        {
            float z1 = heightMap[x][y];
            float z2 = heightMap[x][y + 1];
            float z3 = heightMap[x + 1][y];
            float z4 = heightMap[x + 1][y + 1];

            float color1[3] = {z1, 0.0f, 1.0f - z1};
            float color2[3] = {z2, 0.0f, 1.0f - z2};
            float color3[3] = {z3, 0.0f, 1.0f - z3};
            float color4[3] = {z4, 0.0f, 1.0f - z4};

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z1, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color1[0], color1[1], color1[2]}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color2[0], color2[1], color2[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z3, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color3[0], color3[1], color3[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z3, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {color3[0], color3[1], color3[2]}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), z2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color2[0], color2[1], color2[2]}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), z4, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {color4[0], color4[1], color4[2]}});
        }
    }
}

float smoothstep(float edge0, float edge1, float x)
{
    x = glm::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}

void smoothStepAndAvgRandom(std::vector<vertex> &vertices, std::vector<unsigned int> &indices)
{
    std::vector<std::vector<float>> heightMap(terrainSize, std::vector<float>(terrainSize));
    for (int x = 0; x < terrainSize; ++x)
    {
        for (int y = 0; y < terrainSize; ++y)
        {
            float r = static_cast<float>(rand()) / RAND_MAX;
            heightMap[x][y] = smoothstep(0.0f, 1.0f, r);
        }
    }

    for (int x = 0; x < terrainSize - 1; ++x)
    {
        for (int y = 0; y < terrainSize - 1; ++y)
        {
            float z1 = heightMap[x][y];
            float z2 = heightMap[x][y + 1];
            float z3 = heightMap[x + 1][y];
            float z4 = heightMap[x + 1][y + 1];

            float avg1 = (z1 + z2 + z3) / 3.0f;
            float avg2 = (z3 + z2 + z4) / 3.0f;

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), avg1, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y) / terrainSize},
                                {avg1, 0.0f, 1.0f - avg1}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), avg1, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {avg1, 0.0f, 1.0f - avg1}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), avg1, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {avg1, 0.0f, 1.0f - avg1}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), avg2, static_cast<float>(y - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y) / terrainSize},
                                {avg2, 0.0f, 1.0f - avg2}});

            vertices.push_back({{static_cast<float>(x - terrainHalfSize), avg2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {avg2, 0.0f, 1.0f - avg2}});

            vertices.push_back({{static_cast<float>(x + 1 - terrainHalfSize), avg2, static_cast<float>(y + 1 - terrainHalfSize)},
                                {static_cast<float>(x + 1) / terrainSize, static_cast<float>(y + 1) / terrainSize},
                                {avg2, 0.0f, 1.0f - avg2}});
        }
    }
}

int main()
{
    GLFWwindow *window = createWindow(-1, -1);
    configWindow(window);

    Camera camera(
        glm::vec3(0.0f, 5.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        800.0f / 600.0f,
        0.1f,
        100.0f);
    globalCamera = &camera;

    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    normalRandomWithFixedZ(vertices);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, color));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int shaderProgram = createShaderProgram("shader/terrain/ground.vert", "shader/terrain/ground.frag");

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        view = camera.getViewMatrix();
        projection = camera.getProjectionMatrix();

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
