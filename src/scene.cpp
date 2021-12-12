#include <list>
#include <memory>

#include "renderable.h"
#include "camera.h"

#ifndef SCENE
#define SCENE

#define GRAVITACIA 0.1f
#define VIETOR 0.03f

class Scene
{
public:
	// list of objects
	std::list<std::unique_ptr<Renderable>> objects;

	// camera
	std::unique_ptr<Camera> camera;

	// lights TODO

	// player
	bool jump = false;
	bool move_left = false;
	bool move_right = false;
	glm::vec3 player_position{0, 0, 0};

	float Dtime;

	// update function
	void update(float time)
	{
		Dtime = time;
		auto i = std::begin(objects);
		while (i != std::end(objects))
		{
			// Update object and remove from list if needed
			auto obj = i->get();
			if (!obj->update(time, *this))
				i = objects.erase(i);
			else
				++i;
		}
	}

	// render function
	void render()
	{
		for (auto &object : objects)
		{
			object->render(*this);
		}

		camera->update(player_position);
	}
};

#endif