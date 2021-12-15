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

#ifndef DEER_CHILD
#define DEER_CHILD

class Renderable;

class Deer_child : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f};
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    std::list<std::unique_ptr<Renderable>> objects;

public:
    float size;

    bool right = true;
    bool left = false;

    glm::vec3 position{0, 0, 0};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 scale{size, size, size};

    float bornTime = 0;
    float speed_rotation;

    int child;

    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Deer_child(glm::vec3 p, glm::vec3 c,float s,float d, int e)
    {
        // First particle will initialize resources
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("Deer.obj");

        color = c;
        position = p;

        bornTime = (float)glfwGetTime();

        size = s;

        speed_rotation = d;
        child = e;

        if(e == 1)
        {
            glm::vec3 pos = {0, 0, 2};
            auto deer = std::make_unique<Deer_child>(pos, glm::vec3{0.5070588235294118f,0.4007843137254902f,0.3088235294117647f}, 0.0005f,0.35f, 0);
            objects.push_back(move(deer));
        }
        scale = glm::vec3 {size,size,size};
    }


    bool update_child(float dTime, Scene &scene, glm::mat4 Parent_modelMatrix) override
    {

        bornTime -= speed_rotation;

        modelMatrix = Parent_modelMatrix;
        modelMatrix = glm::rotate(modelMatrix, glm::radians(bornTime), glm::vec3{0, 1, 0});
        modelMatrix = glm::translate(modelMatrix, position);

        auto i = std::begin(objects);
        while (i != std::end(objects))
        {
            // Update object and remove from list if needed
            auto obj = i->get();
            if (!obj->update_child(dTime, scene,modelMatrix))
                i = objects.erase(i);
            else
                ++i;
        }

        modelMatrix = glm::scale(modelMatrix, scale);

        return true;
    }
    bool update(float dTime, Scene &scene) override
    {

    }

    void render(Scene &scene) override
    {

        for (auto &object : objects)
            object->render(scene);

        viewMatrix = scene.camera->viewMatrix;

        shader->use();
        shader->setUniform("OverallColor", color);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", viewMatrix);
        shader->setUniform("ProjectionMatrix", scene.camera->perspective);

        mesh->render();
    }
};

std::unique_ptr<ppgso::Mesh>  Deer_child::mesh;
std::unique_ptr<ppgso::Shader>  Deer_child::shader;

#endif