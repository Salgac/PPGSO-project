
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include <ppgso/ppgso.h>
#include "tree.cpp"

#include "../renderable.h"

class Player final : public Renderable
{
	glm::mat4 viewMatrix{1.0f};
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;
	std::unique_ptr<ppgso::Shader> shader;

public:
	glm::vec3 position{0, 0, 0};
	glm::vec3 speed{0, 0, 0};
	glm::vec3 scale{0.6f, 0.6f, 0.6f};

	/// Construct a new Player
	/// \param p - Initial position
	Player(glm::vec3 p)
	{
		if (!shader)
			shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("player.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("player.obj");

		position = p;
	}

	bool update(float dTime,std::list<std::unique_ptr<Renderable>> &scene) override
	{

        for (auto &object : scene)
        {
            if (object.get() == this)
                continue;
            auto tree = dynamic_cast<Tree*>(object.get());
            if (!tree) continue;

            if (distance(position, tree->position) > tree->scale.y) {
                modelMatrix = glm::translate(modelMatrix, position);
            }
        }

        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{0, 1, 0});
        modelMatrix = glm::scale(modelMatrix, scale);
		return true;
	}

	void render(Camera camera,std::list<std::unique_ptr<Renderable>> &scene) override
	{
		//TODO move according to camera in a better way
		position.z = camera.front.x - 0.6;
		update(0,scene);

		// Render the object
		viewMatrix = camera.viewMatrix;

		shader->use();
		//shader->setUniform("LightDirection", glm::vec3{1.0f, 1.0f, 1.0f});
		shader->setUniform("ModelMatrix", modelMatrix);
		shader->setUniform("ViewMatrix", viewMatrix);
		shader->setUniform("ProjectionMatrix", camera.perspective);

		shader->setUniform("Texture", *texture);

		mesh->render();
	}
};