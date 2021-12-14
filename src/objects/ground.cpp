#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

class Ground final : public Renderable
{
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;

public:
	glm::vec3 position{7, 0, -4};
	glm::vec3 scale{12, 5, 1};

	Ground()
	{
		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("ground.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("quad.obj");
	}

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{-1, 0, 0});
		modelMatrix = glm::scale(modelMatrix, scale);

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
	}

	void setLightShader(Scene &scene)
	{
		glm::mat4 rotationMat(1);
		glm::rotate(rotationMat, glm::radians(90.0f), glm::vec3{1, 0, 0});
		glm::vec3 pos = glm::vec3(rotationMat * glm::vec4(position, 1.0f));

		for (int i = 0; i < scene.LIGHT_COUNT; i++)
		{
			char buffer[64];
			sprintf(buffer, "lights[%d].position", i);
			scene.shader->setUniform(buffer, scene.light_positions.at(i) - pos);
		}
	}
};