
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include "shapes/cube.cpp"

class ProjectWindow : public ppgso::Window
{
private:
	Cube axisX, axisY, axisZ;
	Cube cube;

	// camera stuff
	glm::vec3 cameraPosition{-0.5f, 0.5f, 1.0f};
	glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
	glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
	float cameraSpeed = 0.05f;

public:
	ProjectWindow(int size) : Window{"project", size, size}
	{
		// Set axis colors to red,green and blue...and cube color to grey
		cube.color = {0.5, 0.5, 0.5};
		axisX.color = {1, 0, 0};
		axisY.color = {0, 1, 0};
		axisZ.color = {0, 0, 1};

		const float scaleMin = 0.03f;
		const float scaleMax = 10.00f;

		// Set axis scaling in X,Y,Z directions...hint use scaleMin in tangent directions and scaleMax in the axis direction
		axisX.scale = {scaleMax, scaleMin, scaleMin};
		axisY.scale = {scaleMin, scaleMax, scaleMin};
		axisZ.scale = {scaleMin, scaleMin, scaleMax};
		cube.scale = {0.08, 0.08, 0.08};

		cube.position = {3, 3, 1};
		cube.rotation = {0.0f, 0.0f, 1.0f};
	}

	void onIdle()
	{
		// Set gray background
		glClearColor(.1f, .1f, .1f, 1.0f);
		// Clear depth and color buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Move and Render shape\    // Get time for animation
		float t = (float)glfwGetTime();

		// Set rotation and scale
		cube.rotation.y = sin(t * 0.5f);

		// update view matrix of X,Y,Z axis and cube
		cube.updateViewMatrix(cameraPosition, cameraFront, cameraUp);
		axisX.updateViewMatrix(cameraPosition, cameraFront, cameraUp);
		axisY.updateViewMatrix(cameraPosition, cameraFront, cameraUp);
		axisZ.updateViewMatrix(cameraPosition, cameraFront, cameraUp);

		// update model matrix
		cube.updateModelMatrix();
		axisX.updateModelMatrix();
		axisY.updateModelMatrix();
		axisZ.updateModelMatrix();

		cube.render();
		axisX.render();
		axisY.render();
		axisZ.render();
	}

	void onKey(int key, int scanCode, int action, int mods) override
	{
		if (action == GLFW_PRESS)
		{
			switch (scanCode)
			{
			case 38:
			case 113:
				// left
				cameraFront.x -= cameraSpeed;
				cameraPosition.x -= cameraSpeed;
				break;
			case 40:
			case 114:
				// right
				cameraFront.x += cameraSpeed;
				cameraPosition.x += cameraSpeed;
				break;
			}
		}
	}
};