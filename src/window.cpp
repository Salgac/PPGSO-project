
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

#include "objects/tree.cpp"

#include "camera.h"
#include "scene.cpp"

class ProjectWindow : public ppgso::Window
{
private:
	Scene scene;

	void initScene()
	{
		//TODO create 2 separate templates for scenes
		scene.objects.clear();

		//camera
		auto camera = std::make_unique<Camera>(100.0f, (float)width / (float)height, 0.1f, 100.0f);
		scene.camera = move(camera);

		//player
		scene.objects.push_back(move(std::make_unique<Player>(glm::vec3{0, 0, 0})));

		//backgrounds

        auto cube = std::make_unique<Cube>();
        cube->scale = {0.1f, 0.1f, 0.1f};
        scene.objects.push_back(move(cube));

		scene.objects.push_back(move(std::make_unique<Background>()));
		scene.objects.push_back(move(std::make_unique<Moon>()));
		scene.objects.push_back(move(std::make_unique<Ground>()));

		//trees
		for (int i = 0; i < 35; i++)
		{
			float a = glm::linearRand(-5.0f, -1.0f);
			glm::vec3 pos = glm::vec3{glm::linearRand(-0.5f, 4.0f), 0, a};
			auto tree = std::make_unique<Tree>(pos, glm::vec3{0, -0.01, 0}, glm::vec3{0, 0.5 / (a * a), 0});
			scene.objects.push_back(move(tree));
		}
	}


public:
	ProjectWindow(int size) : Window{"project", size, size}
	{
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        initScene();
	}

	void onIdle()
	{
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Track time
        static auto time = (float)glfwGetTime();
        float dTime = (float)glfwGetTime() - time;
        time = (float)glfwGetTime();

        //update
        scene.update(dTime);

        // Render every object in scene
        scene.render();
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
				scene.camera->front.x -= scene.camera->speed;
				scene.camera->position.x -= scene.camera->speed;
				break;
			case 40:
			case 114:
				// right
				scene.camera->front.x += scene.camera->speed;
				scene.camera->position.x += scene.camera->speed;
				break;
			}
		}
	}
};