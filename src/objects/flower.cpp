#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

class Flower : public Renderable
{
private:
	// Program to associate with the object
	std::unique_ptr<ppgso::Shader> shader;
	std::unique_ptr<ppgso::Mesh> mesh;

	glm::mat4 modelMatrix{1.0f};
	glm::mat4 viewMatrix{1.0f};

public:
	// Public attributes that define position, color ..
	glm::vec3 position;
	glm::vec3 scale{0.005, 0.005, 0.005};
	glm::vec3 color;

	// Initialize object data buffers
	Flower(glm::vec3 pos, glm::vec3 clr, glm::vec3 scl = glm::vec3(1, 1, 1))
	{
		position = pos;
		scale *= scl;
		color = clr;

		// shader
		if (!shader)
			shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("flower.obj");
	};

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		return true;
	}

	// Draw polygons
	void render(Scene &scene) override
	{
		// update viewMatrix
		viewMatrix = scene.camera->viewMatrix;

		// Update transformation and color uniforms in the shader
		shader->use();
		shader->setUniform("OverallColor", color);
		shader->setUniform("ModelMatrix", modelMatrix);
		shader->setUniform("ViewMatrix", viewMatrix);
		shader->setUniform("ProjectionMatrix", scene.camera->perspective);

		mesh->render();
	};
};