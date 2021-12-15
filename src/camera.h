#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#ifndef CAMERA
#define CAMERA

class Camera
{
public:
	glm::mat4 perspective;
	glm::mat4 viewMatrix{1.0f};

	// deprecated
	glm::vec3 position{0.01f, 0.001f, 0.01};
	glm::vec3 front{0.0f, 0.0f, -1.0f};

	glm::vec3 position_offset{0.0f, 0.5f, 1.0f};
	glm::vec3 front_offset{1.0f, 0.0f, -1.0f};
	glm::vec3 up{0.0f, 1.0f, 0.0f};

	glm::vec3 help{0.0f, 1.0f, 0.0f};
	float speed = 0.05f;

	bool go_boundary_right = false;
	bool go_boundary_left = false;
	bool go_player = false;

	const float BOUNDARY_LEFT = -0.5f;
	const float BOUNDARY_RIGHT = 10.0f;

	glm::vec3 boundary_position_right{BOUNDARY_RIGHT, 0.0f, -1.0f};
	glm::vec3 boundary_position_left{BOUNDARY_LEFT, 0.0f, -1.0f};
	/// Representaiton of
	/// \param fov - Field of view (in degrees)
	/// \param ratio - Viewport ratio (width/height)
	/// \param near - Distance of the near clipping plane
	/// \param far - Distance of the far clipping plane
	Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f)
	{
		// Initialize perspective projection
		perspective = glm::perspective((ppgso::PI / 180.f) * fov, ratio, near, far);
	}

	/// Recalculate viewMatrix from position, rotation and scale
	void update(glm::vec3 player_position)
	{
		if (go_boundary_right)
		{
			if (distance(front, boundary_position_right) > 0)
				front.x += position.x;
		}
		if (go_boundary_left)
		{
			if (distance(front, boundary_position_left) > 0)
				front.x -= position.x;
		}
		if (go_player)
		{

			go_boundary_right = false;
			go_boundary_left = false;

			if (distance(front, player_position) > 0.3)
			{
				if (front.x < player_position.x)
					front.x += position.x;
				if (front.x > player_position.x)
					front.x -= position.x;
			}
			else
				go_player = false;
		}
		if (!go_player and !go_boundary_left and !go_boundary_right)
			front = player_position;

		// Update viewMatrix
		if (front.x > BOUNDARY_LEFT && front.x < BOUNDARY_RIGHT)
			viewMatrix = glm::lookAt(front + position_offset, front + front_offset, up);
	}
};

#endif