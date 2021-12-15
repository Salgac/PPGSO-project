#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


    std::unique_ptr<ppgso::Mesh> cornmesh;

class Corn : public Renderable
{
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 modelMatrix{1.0f};
    // Static resources shared between all particles
    std::unique_ptr<ppgso::Shader> shader;

public:
    float size = 0.020f;

    glm::vec3 position{0, 0, 0};
    glm::vec3 scale{size, size, size};
    glm::vec3 color{0.1, 0.5, 0.1};

    Corn(glm::vec3 p)
    {
        position = p;

        // shader
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!cornmesh)
            cornmesh = std::make_unique<ppgso::Mesh>("corn.obj");
    }

    bool update(float dTime, Scene &scene) override
    {
        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{-1, 0, 0});
        modelMatrix = glm::scale(modelMatrix, scale);

        return true;
    }

    void render(Scene &scene) override
    {
        shader->use();
        shader->setUniform("OverallColor", color);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
        shader->setUniform("ProjectionMatrix", scene.camera->perspective);

        cornmesh->render();
    };
};