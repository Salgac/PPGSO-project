#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

#include "../shapes/sphere.cpp"

class Fireflies final : public Renderable
{
	glm::mat4 modelMatrix{1.0f};

	std::list<std::unique_ptr<Sphere>> flies;

public:
	glm::vec3 position{1, 0.5, 0};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 basecolor{0.8, 1, 0.8};
	glm::vec3 color;

	glm::vec3 speed{0.1, 0.1, 0.1};

	Fireflies(glm::vec3 pos, int limit = 25)
	{
		position = pos;

		// init flies into list
		for (int i = 0; i < limit; i++)
		{
			glm::vec3 pos = glm::vec3(glm::linearRand(-0.3, 0.3), glm::linearRand(-0.5, 0.5), glm::linearRand(-0.3, 0.3));
			glm::vec3 rot = glm::vec3(glm::linearRand(-1, 1), glm::linearRand(-1, 1), glm::linearRand(-1, 1));

			auto fly = std::make_unique<Sphere>(position, pos, glm::vec3{0.025, 0.025, 0.025}, color);
			fly->rotation = rot;

			flies.push_back(move(fly));
		}
	}

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		// update position
		if (position.x < 0 || position.x > 0)
			speed.x *= -1;
		if (position.y < 0.5 || position.y > 1)
			speed.y *= -1;
		if (position.z < -2 || position.z > 1)
			speed.z *= -1;

		position += speed * dTime;

		// update color
		float t = glfwGetTime();
		color = basecolor * (float)((sin(t) + 1) / 2.0f + 0.5);

		// iterate through flies
		for (auto &f : flies)
		{
			f->position = position;
			f->color = color;
			f->update(dTime, scene);
		}

		return true;
	}

	void render(Scene &scene) override
	{
		// Render the objects
		scene.shader->use();
		scene.shader->setUniform("ModelMatrix", modelMatrix);
		scene.shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
		scene.shader->setUniform("ProjectionMatrix", scene.camera->perspective);

		// iterate through flies
		for (auto &object : flies)
		{
			object->render(scene);
		}

		// update light
		scene.light_positions.at(2) = position;
		scene.shader->setUniform("lights[2].color", color / glm::vec3(4, 2, 4));
	}
};