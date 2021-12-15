#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

class Moon final : public Renderable
{
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;

public:
	glm::vec3 position{5, 15, -25};
	glm::vec3 scale{5, 5, 1};

	Moon()
	{
		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("moon.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("circle.obj");
	}

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::translate(modelMatrix, position);
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

		// light - only illumination
		scene.shader->setUniform("lights[0].position", glm::vec3{0, 0, 5});
		scene.shader->setUniform("lights[1].position", glm::vec3{0, 0, 5});

		mesh->render();
	}
};