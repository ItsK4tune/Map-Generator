#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <string>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	unsigned int VAO = 0, VBO = 0;

	Mesh() {}
	Mesh(const std::vector<Vertex> &verts);
	~Mesh();

	void draw() const;

private:
	void setupMesh();
};

struct Shader
{
	unsigned int ID = 0;

	Shader();
	Shader(const char *vertexPath, const char *fragmentPath);
	~Shader();

	void use() const;

	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, const glm::vec3 &vec) const;
	void setFloat(const std::string &name, float val) const;

	unsigned int getID() const;
};

class Object
{
public:
	Object(const glm::vec3 &ambient,
		   const glm::vec3 &diffuse,
		   const glm::vec3 &specular,
		   float shininess);

	Object(const Material &material);

	Object(const Mesh &mesh,
		   const glm::vec3 &ambient,
		   const glm::vec3 &diffuse,
		   const glm::vec3 &specular,
		   float shininess);

	Object(const Mesh &mesh,
		   const Material &material);

	Object();

	void setPosition(const glm::vec3 &position);
	void setMesh(const Mesh &mesh);
	void setMaterial(const Material &material);
	void setAmbient(const glm::vec3 &ambient);
	void setDiffuse(const glm::vec3 &diffuse);
	void setSpecular(const glm::vec3 &specular);
	void setShininess(float shininess);
	void setShader(const Shader &shader);

	glm::vec3 getPosition() const;
	Mesh &getMesh();
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	float getShininess() const;
	Shader &getShader();

	void use() const;
	void draw() const;

private:
	glm::vec3 m_position = glm::vec3(0.0f);
	Mesh m_mesh;
	Material m_material;
	Shader m_shader;
};

void generateTerrain(std::vector<Vertex> &vertices, int terrainSize);
void createSunVertices(std::vector<Vertex> &vertices);

#endif