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

class Falling_Tree : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f};
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    float size = 2;

    glm::vec3 position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{size, size, size};
    float rotate = 0;
    float help;

    float rotate_speed = 0.5;
    float y_change_speed;
    float z_change_speed;
    float rotate_angel = 90;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Falling_Tree(glm::vec3 p, glm::vec3 s, glm::vec3 c)
    {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("tree2.obj");

        color = c;
        speed = s;
        position = p;
        help = position.z;

        y_change_speed = abs(position.z - position.y)/ (rotate_angel/rotate_speed);
        z_change_speed = abs(position.z - position.y)/ (rotate_angel/rotate_speed);
    }

    bool update(float dTime, Scene &scene) override
    {
        modelMatrix = glm::mat4{1.0f};

        if(scene.camera->position.x > 1) {

            if (rotate < 90) {
                rotate += rotate_speed;
            }
            if (position.y >= help)
                position.y -= y_change_speed;
            if (position.z <= 0)
                position.z += z_change_speed;
        }

        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate), glm::vec3{1, 0, 0});
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

std::unique_ptr<ppgso::Mesh> Falling_Tree::mesh;
std::unique_ptr<ppgso::Shader> Falling_Tree::shader;