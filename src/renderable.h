
#include "camera.h"

#ifndef RENDERABLE
#define RENDERABLE

class Renderable;                                     // Forward declaration for Scene
using Scene = std::list<std::unique_ptr<Renderable>>;

class Renderable
{
public:
	// Virtual destructor is needed for abstract interfaces
	virtual ~Renderable() = default;

	/// Render the object
	/// \param camera - Camera to use for rendering
	virtual void render(Camera camera) = 0;

	/// Update the object. Useful for specifing animation and behaviour.
	/// \param dTime - Time delta
	/// \param scene - Scene reference
	/// \return - Return true to keep object in scene
	virtual bool update(float dTime) = 0;
};

#endif