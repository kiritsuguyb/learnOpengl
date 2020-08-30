#include "Camera.h"

Camera::Camera()
{
	update(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera::Camera(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up)
{
	
	update(pos,lookat,up);
}


glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

void Camera::LookAt(const glm::vec3& target)
{
	update(position, target, upDir);
}


void Camera::LookAtDir(const glm::vec3 & ldirection)
{
	update(position, position + ldirection, upDir);
}

void Camera::Up(const glm::vec3 & up)
{
	update(position, position + lookDir, up);
}

void Camera::PlaceAt(const glm::vec3 & pos)
{
	update(pos, target, upDir);
}

void Camera::Translate(const glm::vec3 & translation)
{
	auto newpos = position + translation;
	update(newpos, newpos + lookDir, upDir);
}

void Camera::LookAt(float x, float y, float z)
{
	LookAt(glm::vec3(x, y, z));
}

void Camera::Up(float x, float y, float z)
{
	Up(glm::vec3(x, y, z));
}

void Camera::PlaceAt(float x, float y, float z)
{
	PlaceAt(glm::vec3(x, y, z));
}

void Camera::Translate(float x, float y, float z)
{
	Translate(glm::vec3(x, y, z));
}

void Camera::RotateWithMouse(float pitch, float yaw)
{
	auto pitchmat = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f,0.0f,0.0f));
	auto rotateMat = glm::rotate(pitchmat, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	auto lookDir4 = glm::vec4(lookDir, 1.0f);
	lookDir4 = rotateMat * lookDir4;
	lookDir = glm::vec3(lookDir4.x, lookDir4.y, lookDir4.z);
	update(position, position + lookDir, upDir);

}

glm::vec3 Camera::forward()
{
	return glm::normalize(lookDir);
}

glm::vec3 Camera::up()
{
	return glm::normalize(upDir);
}

glm::vec3 Camera::right()
{
	return glm::normalize(rightDir);
}

glm::vec3 Camera::getPos() const
{
	return position;
}

glm::vec3 Camera::getForward() const
{
	return lookDir;
}

void Camera::update(const glm::vec3 & pos, const glm::vec3 & lookat, const glm::vec3 & up)
{
	position = pos;
	target = lookat;
	lookDir = glm::normalize(target - position);
	rightDir = glm::normalize(cross(lookDir, up));
	upDir = glm::normalize(cross(rightDir, lookDir));
	viewMatrix = glm::lookAt(position, position + lookDir, upDir);
}


