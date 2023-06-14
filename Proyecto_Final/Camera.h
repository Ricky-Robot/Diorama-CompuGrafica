#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	bool keyControl(bool* keys, GLfloat deltaTime);
	bool keyControlPersonaje(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void mouseControlPersonaje(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	glm::mat4 calculateViewMatrixPersonaje();
	glm::vec3 getPosicionNueva();
	GLfloat rotCamara() { return rotacionCamara; }

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	glm::vec3 perimetro;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
	GLfloat rotacionCamara;

	void update();
	void update2();
};

