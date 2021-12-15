#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

#include "../scene.cpp"

#include "../renderable.h"
#include "../camera.h"

#ifndef SPHERE
#define SPHERE
// Object to represent a 3D sphere
class Sphere : public Renderable
{
private:
	// Program to associate with the object
	std::unique_ptr<ppgso::Shader> shader;
	std::unique_ptr<ppgso::Mesh> mesh;

	glm::mat4 modelMatrix{1.0f};
	glm::mat4 viewMatrix{1.0f};

	float bornTime;
	float ttl;

public:
	// Public attributes that define position, color ..
	glm::vec3 position;
	glm::vec3 offset;
	glm::vec3 rotation{0, 0, 0};
	glm::vec3 scale{0.0001, 0.0001, 0.0001};
	glm::vec3 desiredScale;

	glm::vec3 color;

	// Initialize object data buffers
	Sphere(glm::vec3 pos, glm::vec3 off, glm::vec3 scl, glm::vec3 clr)
	{
		position = pos;
		offset = off;
		desiredScale = scl;
		color = clr;

		bornTime = (float)glfwGetTime();
		ttl = (float)glm::linearRand(10, 30);

		// shader
		if (!shader)
			shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
		if (!mesh)
			mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
	};

	bool update(float dTime, Scene &scene) override
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), rotation);
		modelMatrix = glm::translate(modelMatrix, offset);
		modelMatrix = glm::scale(modelMatrix, scale);

		// change scale if not desired
		if (scale.x < desiredScale.x)
		{
			scale *= 1.1f;
		}
		else
		{
			scale = desiredScale;
		}

		// check TimeToLive
		if (glfwGetTime() - bornTime > ttl)
		{
			// die
			scale *= 0.8f;
			if (scale.x < 0.0001f)
				return false;
		}
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

#endif