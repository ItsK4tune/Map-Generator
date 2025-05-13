#ifndef LOADSHADER_H
#define LOADSHADER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string loadShaderSource(const char *filepath);
unsigned int compileShader(unsigned int type, const char *source);
unsigned int createShaderProgram(const char *inVert, const char *inFrag);

#endif