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

	// stara kamera glm::vec3 position{-0.5f, 0.5f, 1.0f};
    glm::vec3 position{0.0f, 0.25f, 0.85f};
	glm::vec3 front{0.0f, 0.0f, -1.0f};
	glm::vec3 up{0.0f, 1.0f, 0.0f};
	float speed = 0.05f;

	const float BOUNDARY_LEFT = -0.5f;
	const float BOUNDARY_RIGHT = 10.0f;

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
	void update()
	{
		// Update viewMatrix
		if (front.x > BOUNDARY_LEFT && front.x < BOUNDARY_RIGHT)
			viewMatrix = glm::lookAt(position, front, up);
	}
};

#endif