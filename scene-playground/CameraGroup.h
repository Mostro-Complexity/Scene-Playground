#pragma once
#include "stdafx.h"
#include "Group.h"

class CameraGroup : public Group
{
public:
	void init() override {}

	void render() override
	{
		switch (cameraGroupType)
		{
		case CameraGroupType::roamCamera:
			roamCamera(); break;
		case CameraGroupType::observeCamera:
			observeCamera(); break;
		default:
			break;
		}

	}

	void horizontalRotate(double xpos)
	{
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos); // ˮƽ��
	}

	void verticalRotate(double ypos)
	{
		verticalAngle += mouseSpeed * float(768 / 2 - ypos);// ��ֱ�� 
	}

	void moveForward()
	{
		position += direction * deltaTime * speed;
	}

	void moveBackward()
	{
		position -= direction * deltaTime * speed;
	}

	void strafeRight()
	{
		position += right * deltaTime * speed;
	}

	void strafeLeft()
	{
		position -= right * deltaTime * speed;
	}

	glm::mat4 getProjectionMatrix()
	{
		return ProjectionMatrix;
	}

	glm::mat4 getViewMatrix()
	{
		return ViewMatrix;
	}
private:
	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction;

	float deltaTime;

	// Right vector
	glm::vec3 right;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	CameraGroupType cameraGroupType;

	void getDeltaTime()
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}

	// �������
	void roamCamera() {
		getDeltaTime();

		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

							   // Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	}

	// �۲����
	void observeCamera()
	{
		getDeltaTime();

		position.x = 5 * cos(verticalAngle) * cos(horizontalAngle);
		position.y = 5 * sin(verticalAngle);
		position.z = 5 * cos(verticalAngle) * sin(horizontalAngle);

		float FoV = initialFoV;
		// - 5 * glfwGetMouseWheel();
		// Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		// Projection matrix : 45 degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

		glm::vec3 up;
		if (isUpsideDown(verticalAngle))
			up = glm::vec3(0, -1, 0);// �������¿�,ͷ��Y�Ḻ����
		else
			up = glm::vec3(0, 1, 0);// �������¿�,ͷ��Y��������

									// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			glm::vec3(0., 0., 0.), // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

	// �Ƿ�Ӧ�á��������۲�
	bool isUpsideDown(float verticalAngle)
	{
		// -360�㵽360��ķ�Χ��
		float rangeAngle =
			fmod(verticalAngle, 2 * glm::pi<float>());

		// 90�㵽270��ļ���
		bool set_a = rangeAngle < 3 * glm::pi<float>() / 2
			&& rangeAngle >= glm::pi<float>() / 2;

		// -90�㵽-270��ļ���
		bool set_b = rangeAngle >= -3 * glm::pi<float>() / 2
			&& rangeAngle < -glm::pi<float>() / 2;

		return set_b || set_a;
	}
};
