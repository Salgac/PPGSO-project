#include <iostream>
#include <vector>
#include <list>
#include <memory>

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

class Wolf final : public Renderable {
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    //glm::vec3 color{1.0f};
    // Static resources shared between all particles
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    std::unique_ptr<ppgso::Shader> shader;

public:

    std::list<std::unique_ptr<Wolf>> objects;


    glm::vec3 position{0, 0, 0};
    glm::vec3 start_position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{0.6f, 0.6f, 0.6f};
    glm::vec3 color{1, 1, 0};
    glm::vec3 jump{0, 2.0f, 0};

    float max_dgr = 0;
    float dgr = 0;

    int help_timer = 100;

    bool jumping = false;

    int distance_x = 0;
    int distance_z = 0;

    int child = 0;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Wolf(glm::vec3 p, glm::vec3 s, glm::vec3 c, float r, int d) {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("wolf.obj");
        if (!texture)
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("player.bmp"));

        dgr = d;

        speed = s;
        position = p;
        start_position = p;
        dgr = r;
        color = c;

        //child = det;

        speed.z = 0.3f;
        speed.x = 0.3f;

        objects.clear();
        //create(child);

    }

    /*
    bool create(int child)
    {
        for(int i = 1; i <= child; i++)
        {

            glm::vec3 pos = {1 + i / 2, 0, - 0.5 + i / 2};
            auto wolf1 = std::make_unique<Wolf>(pos, glm::vec3{0, 0, 0}, glm::vec3{0.2 + i * 0.05, 0.2 + i * 0.05, 0.2 + i * 0.05}, 90.0f, 1,0);
            objects.push_back(wolf1);
            objects.push_back(move(wolf1));
        }


    }
     */

    void movement(int random) {
        if (distance_x == 0) {

            distance_x = 100;

            if (random < 25)
                speed.x = 0.2;
            if (random > 25 and random < 50)
                speed.x = -0.2;
            if (random > 50)
                speed.z = 0;
        }
        if (distance_z == 0) {
            int random = rand() % 75;

            distance_z = 100;

            if (random < 25)
                speed.z = 0.2;
            if (random > 25 and random < 50)
                speed.z = -0.2;
            if (random > 50)
                speed.z = 0;

        }
    }

    void move()
    {
        if(position.z > start_position.z + 3 or position.z < start_position.z - 3 )
            speed.z = speed.z * (-1);
        if(position.x > start_position.x + 3 or position.x < start_position.x - 1 or position.x < -3)
            speed.x = speed.x * (-1);

        if(distance_x == 0 or distance_z == 0 )
            movement(rand()%100);

        if (jumping)
        {
            if (speed.y == 0)
                speed += jump;
            jumping = false;
        }

        distance_z--;
        distance_x--;

    }

    bool update(float dTime, Scene &scene) override
    {

        for (auto &obj: scene.objects) {
            // Ignore self in scene
            if (obj.get() == this)
                continue;

            auto collision_object = dynamic_cast<Wolf *>(obj.get());
            if (!collision_object) continue;

            if(glm::distance(position,collision_object->position) < collision_object->scale.x)
            {
                speed.z = (-speed.z);
                speed.x = (-speed.x) ;

                if(speed.z == 0)
                {
                    speed.z = 0.4f;
                    collision_object->speed.z = -0.4f;
                }

               

                collision_object->jumping = true;
                jumping = true;
            }
        }

        move();


        /*
        max_dgr = 0;

        if(speed.x >= 0)
            max_dgr += 0;
        if(speed.x < 0)
            max_dgr -= 180;

        if(speed.z > 0 )
            max_dgr -= 90;
        if(speed.z < 0 )
            max_dgr += 90;

        if(dgr < max_dgr)
            dgr += 1.5f;
        if(dgr > max_dgr)
            dgr -= 1.5f;
        */


        // gravity
        if (position.y >= 0)
        {
            speed.y -= GRAVITACIA;
            position.y += speed.y * dTime;

            //floor when on ground
        }

        if (position.y <  0.01f)
        {
            position.y = 0;
            speed.y = 0;
        }

        position += speed*dTime;

        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{0, 1.0f, 0});
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