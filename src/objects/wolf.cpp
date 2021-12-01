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

class Renderable;

class Wolf final : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    //glm::vec3 color{1.0f};
    // Static resources shared between all particles
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    std::unique_ptr<ppgso::Shader> shader;

public:

    glm::vec3 position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{0.6f, 0.6f, 0.6f};
    glm::vec3 color{1, 1, 0};

    float dgr;
    int drc;
    int help_timer = 0;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Wolf(glm::vec3 p, glm::vec3 s,float r,int d)
    {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("wolf.obj");
        if (!texture)
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("player.bmp"));

        drc = d;

        speed = s;
        position = p;
        dgr = r;


    }

    bool update(float dTime, Scene &scene) override
    {
        for (auto &obj: scene.objects) {
            // Ignore self in scene
            if (obj.get() == this)
                continue;

            auto collision_object = dynamic_cast<Wolf *>(obj.get());
            if (!collision_object) continue;


            if (abs(position.z - collision_object->position.z) < ((scale.y + collision_object->scale.y)/2) )
            {
                help_timer = 200;
                collision_object->help_timer = 200;
            }

            if(help_timer > 0)
            {
                position.z -= (0.3f * drc) * dTime;
                help_timer --;
            }
            else
            {
                position.z += (0.2f * drc) * dTime;
            }
        }





        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::rotate(modelMatrix, glm::radians(dgr), glm::vec3{0, 1, 0});
        modelMatrix = glm::translate(modelMatrix, position);
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