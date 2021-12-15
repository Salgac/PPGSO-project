#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

class Crow final : public Renderable
{
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;

public:
	glm::vec3 position{0, 1, 0};
	glm::vec3 scale{0.02, 0.02, 0.02};

	glm::vec3 speed{0, 0, 0};

	Crow(glm::vec3 pos, glm::vec3 scl = glm::vec3(1, 1, 1))
	{
		position = pos;
		scale *= scl;

		speed = glm::vec3(glm::linearRand(-0.2, 0.2), glm::linearRand(0.1, 0.2), glm::linearRand(0.5, 1.0));

		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("crow.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("crow.obj");
	}

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		// move
		position += speed * dTime;

		// remove if out of scene
		if (position.z > 1)
		{
			return false;
		}

		return true;
	}

	void render(Scene &scene) override
	{
		// Render the object
		scene.shader->use();
		scene.shader->setUniform("ModelMatrix", modelMatrix);
		scene.shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
		scene.shader->setUniform("ProjectionMatrix", scene.camera->perspective);
		scene.shader->setUniform("Texture", *texture);

		// light
		setLightShader(scene);

		mesh->render();
	};

	void setLightShader(Scene &scene)
	{
		for (int i = 0; i < scene.LIGHT_COUNT; i++)
		{
			char buffer[64];
			sprintf(buffer, "lights[%d].position", i);
			scene.shader->setUniform(buffer, scene.light_positions.at(i) - position);
		}
	}
};