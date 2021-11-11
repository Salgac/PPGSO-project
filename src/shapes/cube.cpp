#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Object to represent a 3D cube
class Cube
{
private:
	// 2D vectors define points/vertices of the shape
	// Define cube vertices
	std::vector<glm::vec3> vertices = {
		// front
		{-1, -1, 1},
		{1, -1, 1},
		{1, 1, 1},
		{-1, 1, 1},
		// back
		{-1, -1, -1},
		{1, -1, -1},
		{1, 1, -1},
		{-1, 1, -1},
	};

	// Structure representing a triangular face
	struct Face
	{
		GLuint v0, v1, v2;
	};

	// Indices define triangles that index into vertices
	// Define cube indices
	std::vector<Face> indices = {
		// front
		{0, 1, 2},
		{2, 3, 0},
		// right
		{1, 5, 6},
		{6, 2, 1},
		// back
		{7, 6, 5},
		{5, 4, 7},
		// left
		{4, 0, 3},
		{3, 7, 4},
		// bikini bottom
		{4, 5, 1},
		{1, 0, 4},
		// top
		{3, 2, 6},
		{6, 7, 3},
	};

	// Program to associate with the object
	ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

	// These will hold the data and object buffers
	GLuint vao, vbo, cbo, ibo;
	glm::mat4 modelMatrix{1.0f};
	glm::mat4 viewMatrix{1.0f};

public:
	// Public attributes that define position, color ..
	glm::vec3 position{0, 0, 0};
	glm::vec3 rotation{0, 0, 0};
	glm::vec3 scale{1, 1, 1};
	glm::vec3 color{1, 0, 0};

	// Initialize object data buffers
	Cube()
	{
		// Copy data to OpenGL
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Copy positions to gpu
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		// Set vertex program inputs
		auto position_attrib = program.getAttribLocation("Position");
		glEnableVertexAttribArray(position_attrib);
		glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Copy indices to gpu
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Face), indices.data(), GL_STATIC_DRAW);

		// Set projection matrices to identity
		program.setUniform("ProjectionMatrix", glm::perspective((ppgso::PI / 180.f) * 60.0f, 1.0f, 0.1f, 100.0f));

		program.setUniform("ViewMatrix", viewMatrix);
	};
	// Clean up
	~Cube()
	{
		// Delete data from OpenGL
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &cbo);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	// Set the object transformation matrix
	void updateModelMatrix()
	{
		// Compute transformation by scaling, rotating and then translating the shape
		// Update model matrix: modelMatrix = ... use position, rotation and scale
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = (rotation != glm::vec3(0, 0, 0)) ? glm::rotate(modelMatrix, (float)glfwGetTime(), rotation) : modelMatrix;
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = (rotation != glm::vec3(0, 0, 0)) ? glm::rotate(modelMatrix, (float)glfwGetTime() / 2.0f, {1, 1, 1}) : modelMatrix;
	}

	void updateViewMatrix(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		// lookAt - position, target, up vector
		viewMatrix = glm::lookAt(cameraPosition, cameraFront, cameraUp);
	}

	// Draw polygons
	void render()
	{
		// Update transformation and color uniforms in the shader
		program.use();
		program.setUniform("OverallColor", color);
		program.setUniform("ModelMatrix", modelMatrix);
		program.setUniform("ViewMatrix", viewMatrix);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size() * 3, GL_UNSIGNED_INT, 0);
	};
};