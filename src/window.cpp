
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
#include "objects/player.cpp"
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

		// Set axis colors to red,green and blue...and cube color to grey


        float sizef = 0.5;


        //stromy pred kamerou
        glm::vec3 pos = glm::vec3{1, -0.3, 0.8};
        auto tree = std::make_unique<Tree>(pos, glm::vec3{0.2,0.24,0.2},sizef,2);
        scene.push_back(move(tree));

        // stromy v pozadí
        float xx = 0.3;
        pos = glm::vec3{xx, 0, -0.8};
        tree = std::make_unique<Tree>(pos, glm::vec3{0.18, 0.18, 0.18}, 0.3, 1);
        scene.push_back(move(tree));
        xx = 0;
        pos = glm::vec3{xx, 0.3, -5};
        tree = std::make_unique<Tree>(pos, glm::vec3{0.15, 0.15, 0.15}, 0.2, 0);
        scene.push_back(move(tree));



		auto cube = std::make_unique<Cube>(glm::vec3{-1, -1, -1}, glm::vec3{0.4, 0.4, 0.4});
		auto axisX = std::make_unique<Cube>();
		auto axisY = std::make_unique<Cube>();
		auto axisZ = std::make_unique<Cube>();

		cube->color = {0.5, 0.5, 0.5};
		axisX->color = {0.5, 0.5, 0.5};
		axisY->color = {0, 1, 0};
		axisZ->color = {0, 0, 1};

		const float scaleMin = 0.03f;
		const float scaleMax = 10.00f;

		// Set axis scaling in X,Y,Z directions...hint use scaleMin in tangent directions and scaleMax in the axis direction
		axisX->scale = {scaleMax, scaleMin, 0.7};
		axisY->scale = {scaleMin, scaleMax, scaleMin};
		axisZ->scale = {scaleMin, scaleMin, scaleMax};

		cube->rotation = {0.0f, 0.0f, 1.0f};

		//add into scene
		scene.push_back(move(player));

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