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
	float intensity;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
class Light_Directional:Light
{
public:
	Light_Directional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff,float _intensity=1.0f);
	virtual void applyToShader(const Shader& shader)const;
private:
	glm::vec3 direction;
};
class Light_Point :Light
{
public:
	Light_Point(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, float mdistance = 32.0f,float _minIntensity=0.01f, float _intensity = 1.0f);
	virtual void applyToShader(const Shader& shader)const;
protected:
private:
	glm::vec3 position;
	float maxDistance;
	float minIntesity;
};
class Light_Spot :Light 
{
public:
	Light_Spot(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, float coff = 12.0,float outCoff=20, float mdistance = 32.0f, float _minIntensity = 0.01f, float _intensity = 1.0f);
	virtual void applyToShader(const Shader& shader)const;
private:
	glm::vec3 position;
	glm::vec3 direction;
	float cutoff;
	float outterCutoff;
	float maxDistance;
	float minIntesity;
};
