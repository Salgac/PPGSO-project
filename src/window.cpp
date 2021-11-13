
#include <iostream>
#include <vector>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include "shapes/cube.cpp"
#include "shapes/tree.cpp"
#include "camera.h"

using Scene = std::list<std::unique_ptr<Renderable>>;


class ProjectWindow : public ppgso::Window
{
private:
	Scene scene;
	Camera camera = {100.0f, (float)width / (float)height, 0.1f, 100.0f};

public:
	ProjectWindow(int size) : Window{"project", size, size}
	{
		// Set axis colors to red,green and blue...and cube color to grey

        for (int i = 0; i < 35;i++) {
            float a = glm::linearRand(-5.0f, -1.0f);
            glm::vec3 pos = glm::vec3{glm::linearRand(-0.5f, 4.0f), 0, a};
            auto tree = std::make_unique<Tree>(pos, glm::vec3{0, -0.01, 0}, glm::vec3{0, 0.5/(a*a), 0});
            scene.push_back(move(tree));
        }

		auto cube = std::make_unique<Cube>(glm::vec3{-1, -1, -1}, glm::vec3{0.4, 0.4, 0.4});
		auto axisX = std::make_unique<Cube>();
		auto axisY = std::make_unique<Cube>();
		auto axisZ = std::make_unique<Cube>();

		cube->color = {0.5, 0.5, 0.5};
		axisX->color = {1, 0, 0};
		axisY->color = {0, 1, 0};
		axisZ->color = {0, 0, 1};

		const float scaleMin = 0.03f;
		const float scaleMax = 10.00f;

		// Set axis scaling in X,Y,Z directions...hint use scaleMin in tangent directions and scaleMax in the axis direction
		axisX->scale = {scaleMax, scaleMin, scaleMin};
		axisY->scale = {scaleMin, scaleMax, scaleMin};
		axisZ->scale = {scaleMin, scaleMin, scaleMax};

		cube->rotation = {0.0f, 0.0f, 1.0f};

		//add into scene
		scene.push_back(move(cube));
		scene.push_back(move(axisX));
		scene.push_back(move(axisY));
		scene.push_back(move(axisZ));

		camera.update();
	}

	void onIdle()
	{
		// Set gray background
		glClearColor(.1f, .1f, .1f, 1.0f);
		// Clear depth and color buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Track time
		static auto time = (float)glfwGetTime();
		float dTime = (float)glfwGetTime() - time;
		time = (float)glfwGetTime();

		//update
        auto i = std::begin(scene);
		while (i != std::end(scene))
		{
			// Update object and remove from list if needed
			auto obj = i->get();
			if (!obj->update(dTime))
				i = scene.erase(i);
			else
				++i;
		}

		// Render every object in scene
		for (auto &object : scene)
		{
			object->render(camera);
		}
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
                camera.front.x -= camera.speed;
				camera.position.x -= camera.speed;
				break;
			case 40:
			case 114:
				// right
				camera.front.x += camera.speed;
				camera.position.x += camera.speed;
				break;
			}
		}

		//update camera
		camera.update();
	}
};