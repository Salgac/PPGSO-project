
#include <iostream>
#include <vector>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include "shapes/cube.cpp"
#include "objects/player.cpp"
#include "objects/background.cpp"
#include "objects/moon.cpp"
#include "objects/ground.cpp"

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
		auto player = std::make_unique<Player>(glm::vec3{0, 0, 0});

		auto background = std::make_unique<Background>();
		auto moon = std::make_unique<Moon>();
		auto floor = std::make_unique<Ground>();

		auto axisX = std::make_unique<Cube>();
		auto axisY = std::make_unique<Cube>();
		auto axisZ = std::make_unique<Cube>();

		axisX->color = {1, 0, 0};
		axisY->color = {0, 1, 0};
		axisZ->color = {0, 0, 1};

		const float scaleMin = 0.03f;
		const float scaleMax = 10.00f;

		// Set axis scaling in X,Y,Z directions...hint use scaleMin in tangent directions and scaleMax in the axis direction
		axisX->scale = {scaleMax, scaleMin, scaleMin};
		axisY->scale = {scaleMin, scaleMax, scaleMin};
		axisZ->scale = {scaleMin, scaleMin, scaleMax};

		//add into scene
		scene.push_back(move(player));
		scene.push_back(move(background));
		scene.push_back(move(moon));
		scene.push_back(move(floor));

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