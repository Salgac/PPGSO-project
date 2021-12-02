
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include <ppgso/ppgso.h>

#include "../renderable.h"
#include "../scene.cpp"

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

	glm::vec3 jump{0, 2.0f, 0};

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

	bool update(float dTime, Scene &scene) override
	{
		// collisions
		for (auto &obj : scene.objects)
		{
			// Ignore self in scene
			if (obj.get() == this)
				continue;

			auto collision_object = dynamic_cast<Cube *>(obj.get());
			if (!collision_object)
				continue;

			if (abs(position.z + collision_object->position.z) < (scale.y + collision_object->scale.y) / 2 && collision_object->position.y + collision_object->scale.y / 2 > position.y)
			{
				if (position.z < collision_object->position.z)
				{
					position.z -= scene.camera->speed;
					scene.camera->front.x -= scene.camera->speed;
					scene.camera->position.x -= scene.camera->speed;
				}
				if (position.z > collision_object->position.z)
				{
					position.z += scene.camera->speed;
					scene.camera->front.x += scene.camera->speed;
					scene.camera->position.x += scene.camera->speed;
				}
			}
			//from top
			if (abs(position.y + collision_object->position.y) < collision_object->scale.y + collision_object->position.y && abs(position.z + collision_object->position.z) < (collision_object->scale.z + scale.z) / 2 + collision_object->position.z)
			{
				if (position.y > collision_object->position.y)
					position.y = collision_object->position.y + collision_object->scale.y;
				dTime = 0;
			}
		}

		// move the player
		position.z = scene.camera->front.x - 0.6;

		// jumps
		if (scene.jump)
		{
			if (speed.y == 0)
				speed += jump;
			scene.jump = false;
		}

		// gravity
		if (position.y >= 0)
		{
			speed.y -= GRAVITACIA;
			position.y += speed.y * dTime;

			//floor when on ground
			if (position.y < 0.01f)
			{
				position.y = 0;
				speed.y = 0;
			}
		}

		// generate modelMatrix
		modelMatrix = glm::mat4{1.0f};
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{0, 1, 0});
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);

		return true;
	}
	void render(Scene &scene) override
	{
		// Render the object
		viewMatrix = scene.camera->viewMatrix;

		shader->use();
		//shader->setUniform("LightDirection", glm::vec3{1.0f, 1.0f, 1.0f});
		shader->setUniform("ModelMatrix", modelMatrix);
		shader->setUniform("ViewMatrix", viewMatrix);
		shader->setUniform("ProjectionMatrix", scene.camera->perspective);

		shader->setUniform("Texture", *texture);

		mesh->render();
	};
};
