#include "Light.h"

Light_Directional::Light_Directional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff,float _intensity)
{
	direction = dir;
	ambient = amb;
	diffuse = diff;
	intensity = _intensity;
}

void Light_Directional::applyToShader(const Shader & shader) const
{
	shader.setVec3("light.direction", glm::value_ptr(direction));
	shader.setVec3("light.ambient", glm::value_ptr(ambient));
	shader.setVec3("light.diffuse", glm::value_ptr(diffuse));
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("light.intensity", intensity);
}

Light::Light()
{
}

Light::~Light()
{
}

Light_Point::Light_Point(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, float mdistance,float _minIntensity, float _intensity)
{
	position = pos;
	ambient = amb;
	diffuse = diff;
	maxDistance = mdistance;
	minIntesity = _minIntensity;
	intensity = _intensity;
}

void Light_Point::applyToShader(const Shader & shader) const
{
	float k = ((1 / minIntesity) - 1) / maxDistance;

	shader.setVec3("light.position", glm::value_ptr(position));
	shader.setVec3("light.ambient", glm::value_ptr(ambient));
	shader.setVec3("light.diffuse", glm::value_ptr(diffuse));
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("light.k", k);
	shader.setFloat("light.minIntensity", minIntesity);
	shader.setFloat("light.intensity", intensity);
	shader.setFloat("light.maxDistance", maxDistance);
}

Light_Spot::Light_Spot(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, float coff , float outCoff, float mdistance, float _minIntensity, float _intensity)
{
	position = pos;
	direction = dir;
	ambient = amb;
	diffuse = diff;
	cutoff = coff;
	outterCutoff = outCoff;
	maxDistance = mdistance;
	minIntesity = _minIntensity;
	intensity = _intensity;
}

void Light_Spot::applyToShader(const Shader & shader) const
{
	float k = ((1.0f / minIntesity) - 1) / maxDistance;
	shader.setVec3("light.position", glm::value_ptr(position));
	shader.setVec3("light.direction", glm::value_ptr(direction));
	shader.setVec3("light.ambient", glm::value_ptr(ambient));
	shader.setVec3("light.diffuse", glm::value_ptr(diffuse));
	shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("light.k", k);
	shader.setFloat("light.minIntensity", minIntesity);
	shader.setFloat("light.intensity", intensity);
	shader.setFloat("light.maxDistance", maxDistance);
	shader.setFloat("light.cutoff", glm::cos(glm::radians(cutoff)));
	shader.setFloat("light.outterCutoff", glm::cos(glm::radians(outterCutoff)));
}
