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

public:
	// Public attributes that define position, color ..
	glm::vec3 position;
	glm::vec3 rotation{0, 0, 0};
	glm::vec3 scale;
	glm::vec3 color;

	// Initialize object data buffers
	Sphere(glm::vec3 pos, glm::vec3 scl, glm::vec3 clr)
	{
		position = pos;
		scale = scl;
		color = clr;

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

#endif