#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

bool Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
		return true;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	return false;
}

bool Camera::keyControlPersonaje(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position -= perimetro * velocity;
		return true;
	}

	if (keys[GLFW_KEY_S])
	{
		position += perimetro * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position += glm::normalize(glm::cross(perimetro, worldUp)) * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position -= glm::normalize(glm::cross(perimetro, worldUp)) * velocity;
	}

	return false;
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}
void Camera::mouseControlPersonaje(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update2();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::calculateViewMatrixPersonaje()
{
	return glm::lookAt(position + perimetro * glm::vec3(10.0f, 10.0f, 10.0f), position, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

glm::vec3 Camera::getPosicionNueva()
{
	return position + perimetro * glm::vec3(10.0f, 10.0f, 10.0f);
}


void Camera::update2()
{
	rotacionCamara = yaw;
	perimetro.x = cos(glm::radians(yaw));
	perimetro.y = 0.0f;
	perimetro.z = sin(glm::radians(yaw));
	perimetro = glm::normalize(perimetro);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
