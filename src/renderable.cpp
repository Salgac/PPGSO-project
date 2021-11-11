class Renderable
{
public:
	// Virtual destructor is needed for abstract interfaces
	virtual ~Renderable() = default;

	/// Render the object
	/// \param camera - Camera to use for rendering
	virtual void render(glm::vec3 cameraPosition, glm::vec3 cameraFront) = 0;

	/// Update the object. Useful for specifing animation and behaviour.
	/// \param dTime - Time delta
	/// \param scene - Scene reference
	/// \return - Return true to keep object in scene
	virtual bool update(float dTime) = 0;
};