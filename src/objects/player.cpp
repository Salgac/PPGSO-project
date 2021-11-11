
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

#include <ppgso/ppgso.h>

#include "../renderable.h"

class Player final : public Renderable
{
	glm::mat4 viewMatrix{1.0f};
	glm::mat4 modelMatrix{1.0f};
	glm::vec3 color{1, 1, 0};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Shader> shader;

public:
	glm::vec3 position{0, 0, 0};
	glm::vec3 speed{0, 0, 0};
	glm::vec3 scale{0.003, 0.003, 0.003};

	/// Construct a new Player
	/// \param p - Initial position
	/// \param s - Initial speed
	/// \param c - Color of particle
	Player(glm::vec3 p)
	{
		if (!shader)
			shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("player.obj");

		position = p;
	}

	bool update(float dTime) override
	{
		// Animate position using speed and dTime.
		// - Return true to keep the object alive
		// - Returning false removes the object from the scene
		// - hint: you can add more particles to the scene here also
		//modelMatrix = glm::translate(modelMatrix, speed);

		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{0, 1, 0});
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		return true;
	}

	void render(Camera camera) override
	{
		// Render the object
		viewMatrix = camera.viewMatrix;

		// Update transformation and color uniforms in the shader
		shader->use();
		shader->setUniform("OverallColor", color);
		shader->setUniform("ModelMatrix", modelMatrix);
		shader->setUniform("ViewMatrix", viewMatrix);
		shader->setUniform("ProjectionMatrix", camera.perspective);

		mesh->render();
	}
};