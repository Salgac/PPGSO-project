
#include <iostream>
#include <vector>
#include <list>

#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

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
#include "objects/deer.cpp"

#include "camera.h"
#include "scene.cpp"

#include <shaders/convolution_vert_glsl.h>
#include <shaders/convolution_frag_glsl.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

class ProjectWindow : public ppgso::Window
{
private:
	Scene scene;

	// Objects to render the framebuffer on to
	// ppgso::Shader quadShader = {convolution_vert_glsl, convolution_frag_glsl};
	ppgso::Shader quadShader = {texture_vert_glsl, texture_frag_glsl};
	ppgso::Mesh quadMesh = {"quad.obj"};
	ppgso::Texture quadTexture = {1024, 1024};

	// OpenGL object ids for framebuffer and render buffer
	GLuint fbo = 1;
	GLuint rbo = 1;

	void scene1_init() // scene 1
	{
		scene.objects.clear();

		// camera
		auto camera = std::make_unique<Camera>(100.0f, (float)width / (float)height, 0.1f, 100.0f);
		scene.camera = move(camera);

		// shader and light
		auto shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);

		// moonlight
		scene.light_positions.push_back(glm::vec3(5, 7, -13));
		shader->setUniform("lights[0].color", glm::vec3(1, 0.5, 0.5));

		// ambient
		scene.light_positions.push_back(glm::vec3(0, 2, 2));
		shader->setUniform("lights[1].color", glm::vec3(0.3, 0.3, 0.3));

		// tmp - for fireflies
		glm::vec3 posc = glm::vec3(8, 1, -2);
		scene.objects.push_back(move(std::make_unique<Cube>(posc, glm::vec3(0.1, 0.1, 0.1), 1)));
		scene.light_positions.push_back(posc);
		shader->setUniform("lights[2].color", glm::vec3(0.1, 0.3, 0.1));

		scene.shader = move(shader);

		// player
		scene.objects.push_back(move(std::make_unique<Player>(glm::vec3{0, 1, 0})));

		// backgrounds
		scene.objects.push_back(move(std::make_unique<Background>()));
		scene.objects.push_back(move(std::make_unique<Moon>()));
		scene.objects.push_back(move(std::make_unique<Ground>()));

		// trees
		for (int i = 0; i < 50; i++)
		{
			float a = glm::linearRand(-3.0f, -12.0f);
			glm::vec3 pos = glm::vec3{glm::linearRand(-2.0f, 12.0f), 0, a};
			auto tree = std::make_unique<Tree>(pos, glm::vec3{0, -0.01, 0}, glm::vec3{0, 2.5 / (a * a), 0});
			scene.objects.push_back(move(tree));
		}

		// faling trees
		for (int i = 0; i < 3; i++)
		{
			glm::vec3 pos = glm::vec3{4 + (i * 2), 0, -1.5};
			auto tree = std::make_unique<Falling_Tree>(pos, glm::vec3{0, -0.01, 0});
			scene.objects.push_back(move(tree));
		}

		// wolfs
		for (float i = 0; i < 5; i++)
		{
			glm::vec3 pos = {glm::linearRand(12.0f, 18.0f), 0, glm::linearRand(-1.5f, -4.0f)};
			auto wolf1 = std::make_unique<Wolf>(pos, glm::vec3{0, 0, 0}, glm::vec3{0.2 + i * 0.05, 0.2 + i * 0.05, 0.2 + i * 0.05}, 90.0f, 1);
			scene.objects.push_back(move(wolf1));
		}

		// deer
		glm::vec3 pos = {10, 0, -2};
		auto deer = std::make_unique<Deer>(pos, glm::vec3{1, 1, 0});
		scene.objects.push_back(move(deer));
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
		if (scene.scene_count == 1)
			scene1_init();
		if (scene.scene_count == 2)
			scene2_init();
	}

public:
	ProjectWindow(int size) : Window{"project", size, size}
	{

		buffer_init();
		initScene();
	}

	void buffer_init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		quadTexture.bind();

		unsigned int framebufferTexture;
		glGenTextures(1, &framebufferTexture);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

		// Initialize framebuffer, its color texture (the sphere will be rendered to it) and its render buffer for depth info storage
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Set up render buffer that has a depth buffer and stencil buffer
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Associate the quadTexture with it
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, quadTexture.image.width, quadTexture.image.height);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, quadTexture.getTexture(), 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Cannot create framebuffer!");
	}

	void buffer_set()
	{
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glClearColor(.5f, .7f, .5f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void buffer_show()
	{
		resetViewport();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Clear the framebuffer
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto quadViewMatrix = glm::mat4{1.0f};
		quadViewMatrix = glm::lookAt(glm::vec3{0.0f, 0.0f, -0.8f}, scene.camera->help - glm::vec3{0.0f, 1.0f, -1.0f}, {0.0f, -1.0f, 0.0f});

		// Animate rotation of the quad
		auto quadModelMatrix = glm::mat4{1.0f};

		// Set shader inputs
		quadShader.use();
		quadShader.setUniform("ProjectionMatrix", scene.camera->perspective);
		quadShader.setUniform("ViewMatrix", quadViewMatrix);
		quadShader.setUniform("ModelMatrix", quadModelMatrix);
		quadShader.setUniform("Texture", quadTexture);
		quadMesh.render();
	}

	void onIdle()
	{
		buffer_set();

		// Track time
		static auto time = (float)glfwGetTime();
		float dTime = (float)glfwGetTime() - time;
		time = (float)glfwGetTime();

		// update
		scene.update(dTime);

		// Render every object in scene
		scene.render();

		if (scene.objects.size() == 0)
			initScene();

		buffer_show();
	}

	void onKey(int key, int scanCode, int action, int mods) override
	{
		if (action == GLFW_PRESS)
		{
			std::cout << scanCode;
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