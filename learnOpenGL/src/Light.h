#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
class Light
{
public:
	Light();
	~Light();
	virtual void applyToShader(const Shader& shader)const=0;

protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
class Light_Directional:Light
{
public:
	Light_Directional(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff);
	virtual void applyToShader(const Shader& shader)const;
private:
	glm::vec3 direction;
};

