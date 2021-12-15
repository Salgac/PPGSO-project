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

#include "../scene.cpp"
#include "../renderable.h"
#include "../camera.h"
class Renderable;

class Deer : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f};
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    float size = 0.002f;

    bool right = true;
    bool left = false;

    glm::vec3 position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{size, size, size};
    float bornTime = 0;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Deer(glm::vec3 p, glm::vec3 c)
    {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("Deer.obj");

        color = c;
        position = p;

        bornTime = (float)glfwGetTime();
    }

    bool update(float dTime, Scene &scene) override
    {
        // Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        //modelMatrix = glm::translate(modelMatrix, speed);

        //new pos
        //if (position.y > 0)
        //    position += speed;



        if(right)
            bornTime += 0.05;
        if(bornTime > 90)
        {
            right = false;
            left = true;
        }

        if(left)
        {
            bornTime -= 0.05;
        }

        if(bornTime < -90)
        {
            right = true;
            left = false;
        }

        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(bornTime ), glm::vec3{0, 1, 0});

        modelMatrix = glm::scale(modelMatrix, scale);

        return true;


    }

    void render(Scene &scene) override
    {
        // Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh

        viewMatrix = scene.camera->viewMatrix;
        // Update transformation and color uniforms in the shader
        shader->use();
        shader->setUniform("OverallColor", color);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", viewMatrix);
        shader->setUniform("ProjectionMatrix", scene.camera->perspective);

        mesh->render();
    }
};

std::unique_ptr<ppgso::Mesh> Deer::mesh;
std::unique_ptr<ppgso::Shader> Deer::shader;