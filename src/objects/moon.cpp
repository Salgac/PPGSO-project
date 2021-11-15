
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include <ppgso/ppgso.h>

#include "../renderable.h"

class Moon final : public Renderable
{
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;
	std::unique_ptr<ppgso::Shader> shader;

public:
	glm::vec3 position{5, 15, -25};
	glm::vec3 scale{5, 5, 0};

	Moon()
	{
		if (!shader)
			shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("moon.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("circle.obj");
	}

	bool update(float dTime) override
	{
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		return true;
	}

	void render(Camera camera) override
	{
		// Render the object
		shader->use();
		shader->setUniform("ModelMatrix", modelMatrix);
		shader->setUniform("ViewMatrix", camera.viewMatrix);
		shader->setUniform("ProjectionMatrix", camera.perspective);
		shader->setUniform("Texture", *texture);
		shader->setUniform("TextureOffset", glm::vec2(0.0f, 0.0f));

		mesh->render();
	}
};