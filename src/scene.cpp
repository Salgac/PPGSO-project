#include <list>
#include <memory>

#include "renderable.h"
#include "camera.h"

class Scene
{
public:
	//list of objects
	std::list<std::unique_ptr<Renderable>> objects;

	//camera
	std::unique_ptr<Camera> camera;

	//lights TODO

	//update function
	void update(float time)
	{
		auto i = std::begin(objects);
		while (i != std::end(objects))
		{
			// Update object and remove from list if needed
			auto obj = i->get();
			if (!obj->update(time))
				i = objects.erase(i);
			else
				++i;
		}
	}

	//render function
	void render()
	{
		camera->update();

		for (auto &object : objects)
		{
			object->render(*camera);

			//TODO redo renderable definition
			//object->render(*this);
		}
	}
};