#include <ppgso/ppgso.h>

#include "../renderable.h"
#include "../scene.cpp"

#include "falling_tree.cpp"

class Player final : public Renderable
{
	glm::mat4 viewMatrix{1.0f};
	glm::mat4 modelMatrix{1.0f};

	// Static resources
	std::unique_ptr<ppgso::Mesh> mesh;
	std::unique_ptr<ppgso::Texture> texture;

public:
	glm::vec3 position{0, 0, 0};
	glm::vec3 speed{0, 0, 0};
	glm::vec3 scale{0.6f, 0.6f, 0.6f};

	glm::vec3 jump{0, 2.0f, 0};
	glm::vec3 move{0, 0, 0.5f};

	float ground = 0;

	/// Construct a new Player
	/// \param p - Initial position
	Player(glm::vec3 p)
	{
		if (!texture)
			texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("player.bmp"));
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("player.obj");

		position = p;
	}
	bool update(float dTime, Scene &scene) override
	{
		// collisions
		glm::vec3 player_position{position.z, position.y, position.x};
		for (auto &obj : scene.objects)
		{
			// Ignore self in scene
			if (obj.get() == this)
				continue;

			auto collision_object = dynamic_cast<Cube *>(obj.get());
			if (!collision_object)
				continue;

			if (glm::distance(player_position, collision_object->position) < collision_object->scale.x + scale.z / 2)
			{
				if (player_position.y < collision_object->scale.y)
				{
					if (player_position.x < collision_object->position.x)
						scene.move_right = false;

					if (player_position.x > collision_object->position.x)
						scene.move_left = false;
				}

				else
				{
					if (!scene.jump)
						ground = collision_object->position.y + collision_object->scale.y;
				}
			}
			else if (glm::distance(player_position, collision_object->position) > collision_object->scale.x + scale.z / 2 + 0.025f)
				ground = 0;
		}

		// jumps
		if (scene.jump)
		{
			if (speed.y == ground)
				speed += jump;
			scene.jump = false;
		}

		// gravity
		if (position.y >= ground)
		{
			speed.y -= GRAVITACIA;
			position.y += speed.y * dTime;

			// floor when on ground
			if (position.y < ground + 0.01f)
			{
				position.y = ground;
				speed.y = 0;
			}
		}

		// move the player
		if (scene.move_left)
		{
			if (abs(speed.z) == 0)
				speed -= move;
		}
		else if (scene.move_right)
		{
			if (abs(speed.z) == 0)
				speed += move;
		}
		else
		{
			speed.z = 0;
		}

		if (position.y > 0)
		{
			position.z -= VIETOR * dTime;
		}

		position.z += speed.z * dTime;

		// for scene specific actions
		scene.player_position = position;

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

		// shader
		scene.shader->use();
		scene.shader->setUniform("ModelMatrix", modelMatrix);
		scene.shader->setUniform("ViewMatrix", viewMatrix);
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
