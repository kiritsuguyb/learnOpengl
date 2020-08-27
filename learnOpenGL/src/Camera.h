#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
public:
	Camera();
	Camera(const glm::vec3& pos, const glm::vec3 &lookat=glm::vec3(0.0f,0.0f,0.0f), const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 getViewMatrix();
	void LookAt(const glm::vec3& target);
	void LookAtDir(const glm::vec3& target);
	void Up(const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	void PlaceAt(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 1.0f));
	void Translate(const glm::vec3& translation = glm::vec3(0.0f, 0.0f, 0.0f));
	void LookAt(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void Up(float x=0.0f, float y=1.0f, float z=0.0f);
	void PlaceAt(float x = 0.0f, float y = 0.0f, float z = 1.0f);
	void Translate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void RotateWithMouse(float pitch, float yaw);
	glm::vec3 forward();
	glm::vec3 up();
	glm::vec3 right();
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 lookDir;
	glm::vec3 upDir;
	glm::vec3 rightDir;
	glm::mat4 viewMatrix;
	void update(const glm::vec3& pos, const glm::vec3 &lookat, const glm::vec3 &up);
};

