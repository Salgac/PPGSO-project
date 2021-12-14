
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
#include "objects/falling_tree.cpp"
#include "objects/wolf.cpp"

#include "camera.h"
#include "scene.cpp"

class ProjectWindow : public ppgso::Window
{
private:
	Scene scene;


    void scene1_init()
    {
        scene.objects.clear();


        // camera
        auto camera = std::make_unique<Camera>(100.0f, (float)width / (float)height, 0.1f, 100.0f);
        scene.camera = move(camera);

        // player
        scene.objects.push_back(move(std::make_unique<Player>(glm::vec3{0, 1, 0})));

        // backgrounds
        scene.objects.push_back(move(std::make_unique<Background>()));
        scene.objects.push_back(move(std::make_unique<Moon>()));
        scene.objects.push_back(move(std::make_unique<Ground>()));

        // trees
        for (int i = 0; i < 30; i++)
        {
            float a = glm::linearRand(-3.0f, -10.0f);
            glm::vec3 pos = glm::vec3{glm::linearRand(-2.0f, 15.0f), 0, a};
            auto tree = std::make_unique<Tree>(pos, glm::vec3{0, -0.01, 0}, glm::vec3{0, 2.5 / (a * a), 0});
            scene.objects.push_back(move(tree));
        }


        for (int i = 0; i < 3;i++)
        {
            glm::vec3 pos = glm::vec3{4+(i*2), 0, -1.5};
            auto tree = std::make_unique<Falling_Tree>(pos, glm::vec3{0, -0.01, 0}, glm::vec3{0, 0.4, 0});
            scene.objects.push_back(move(tree));
        }
        // wolfs
        for (float i = 0; i < 5; i++)
        {
            glm::vec3 pos = {2.5 + i / 2, 0, 8.5 + i / 2};
            auto wolf1 = std::make_unique<Wolf>(pos, glm::vec3{0, 0, 0}, glm::vec3{0.2 + i * 0.05, 0.2 + i * 0.05, 0.2 + i * 0.05}, 90.0f, 1);
            scene.objects.push_back(move(wolf1));
        }

    }

    void scene2_init()
    {
        scene.objects.clear();


        // camera
        auto camera = std::make_unique<Camera>(100.0f, (float)width / (float)height, 0.1f, 100.0f);
        scene.camera = move(camera);

        // player
        scene.objects.push_back(move(std::make_unique<Player>(glm::vec3{0, 1, 0})));

        // backgrounds
        scene.objects.push_back(move(std::make_unique<Background>()));
        scene.objects.push_back(move(std::make_unique<Moon>()));
        scene.objects.push_back(move(std::make_unique<Ground>()));

    }


	void initScene()
	{
        scene.scene_count++;
        if( scene.scene_count == 1)
            scene1_init();
        if( scene.scene_count == 2)
            scene2_init();
	}

public:
	ProjectWindow(int size) : Window{"project", size, size}
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		initScene();
	}

	void onIdle() {
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Track time
        static auto time = (float) glfwGetTime();
        float dTime = (float) glfwGetTime() - time;
        time = (float) glfwGetTime();

        // update
        scene.update(dTime);

        // Render every object in scene
        scene.render();

        if(scene.objects.size() == 0)
            initScene();

    }

	void onKey(int key, int scanCode, int action, int mods) override
	{
		if (action == GLFW_PRESS)
		{
            std:: cout << scanCode;
			switch (scanCode)
			{
			case 38:
			case 113:
				// left
				scene.move_left = true;
				break;
			case 40:
			case 114:
				// right
				scene.move_right = true;
				break;
			case 65:
				// spacebar
				if (!scene.jump)
					scene.jump = true;
				break;
            case 77:
                scene.camera->go_boundary_right = true;
                scene.camera->go_boundary_left = false;
                break;
            case 75:
                scene.camera->go_boundary_left = true;
                scene.camera->go_boundary_right = false;
                break;
            case 76:
                scene.camera->go_boundary_left = false;
                scene.camera->go_boundary_right = false;
                scene.camera->go_player = true;
                break;
			}
		}
		if (action == GLFW_RELEASE)
		{
			switch (scanCode)
			{
			case 38:
			case 113:
				// left
				scene.move_left = false;
				break;
			case 40:
			case 114:
				// right
				scene.move_right = false;
				break;
			}
		}
	}
};