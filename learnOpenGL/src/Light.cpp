#include "Light.h"

Light_Directional::Light_Directional(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff)
{
	direction = dir;
	ambient = amb;
	diffuse = diff;
}

void Light_Directional::applyToShader(const Shader & shader) const
{
	shader.setVec3("lightDirectional.direction", glm::value_ptr(direction));
	shader.setVec3("lightDirectional.ambient", glm::value_ptr(ambient));
	shader.setVec3("lightDirectional.diffuse", glm::value_ptr(diffuse));
	shader.setVec3("lightDirectional.specular", 1.0f, 1.0f, 1.0f);
}

Light::Light()
{
}

Light::~Light()
{
}
