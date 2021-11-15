#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
//#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <list>

#include "src/renderable.h"
#include "src/camera.h"
class Renderable;
using Scene = std::list<std::unique_ptr<Renderable>>;

class Tree : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f};
    // Static resources shared between all particles
    std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    float size = glm::linearRand(0.025f, 0.075f);

    glm::vec3 position{0, 0, 0};
    //glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{size, size, size};
    float bornTime;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Tree(glm::vec3 p, glm::vec3 c,float d,int e)
    {
        // First particle will initialize resources
        //std::cout << "Thanks for viewing my code!";
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (e == 0)
            mesh = std::make_unique<ppgso::Mesh>("../data/Tree.obj");
        if (e == 1)
            mesh = std::make_unique<ppgso::Mesh>("../data/Tree2.obj");
        if (e == 2)
            mesh = std::make_unique<ppgso::Mesh>("../data/Tree3.obj");

        color = c;
        position = p;

        size = d;
        scale = {size, size, size};

        bornTime = (float)glfwGetTime();
    }

    bool update(float dTime) override
    {
        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);

        if (dTime - bornTime < 15)
            return true;
        else
            return false;
    }

    void render(Camera camera) override
    {
        // Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh

        viewMatrix = camera.viewMatrix;
        // Update transformation and color uniforms in the shader
        shader->use();
        shader->setUniform("OverallColor", color);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", viewMatrix);
        shader->setUniform("ProjectionMatrix", camera.perspective);

        mesh->render();
    }
};

//std::unique_ptr<ppgso::Mesh> Tree::mesh;
std::unique_ptr<ppgso::Shader> Tree::shader;