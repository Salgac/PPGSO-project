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

#include "../shapes/cube.cpp"
#include "particle.cpp"

#ifndef FALL_TREE
#define FALL_TREE

class Renderable;

class Falling_Tree : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{0.3607843137254902f, 0.2509803921568627f, 0.2f};

    //0.4470588235294118f,0.3607843137254902f,0.2588235294117647f
    //0.3607843137254902f, 0.2509803921568627f, 0.2f

    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    float size = 3;

    glm::vec3 position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{size, size, size};

    bool particles = false;
    bool collision = false;
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
    Falling_Tree(glm::vec3 p, glm::vec3 s)
    {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("tree2.obj");
        speed = s;
        position = p;
        help = position.z;

        y_change_speed = abs(position.z - position.y)/ (rotate_angel/rotate_speed);
        z_change_speed = abs(position.z - position.y)/ (rotate_angel/rotate_speed);
    }

    bool update(float dTime, Scene &scene) override
    {
        modelMatrix = glm::mat4{1.0f};
        
        if( position.x - scene.player_position.z < 1.5) {

            if(!collision)
            {
                collision = true;
                particles = true;
                auto cube = std::make_unique<Cube>(glm::vec3{position.x, 0, 0}, glm::vec3{0, 0.3, 1},0);
                cube->scale = {0.15f, 0.15, 0.15};
                scene.objects.push_back(move(cube));
            }

            if (rotate < 90) {
                rotate += rotate_speed;

            }
            else
            {
                if(particles)
                {
                    for(int i = 0; i < 10; i++)
                    {
                        glm::vec3 pst;
                        pst.x = glm::linearRand(position.x - 0.2 , position.x + 0.2);
                        pst.y = 0.5;
                        pst.z = 0;

                        auto prtcl = std::make_unique<Particle>(pst,glm::vec3{0,0,0},glm::vec3{0,1,0});
                        prtcl->scale = {0.05f, 0.05, 0.05};
                        scene.objects.push_back(move(prtcl));
                    }


                    particles = false;
                }
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

#endif
