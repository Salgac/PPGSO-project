#include <ppgso/ppgso.h>
#include "../scene.cpp"
#include "../renderable.h"
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
class Lake final : public Renderable
{
    glm::mat4 modelMatrix{1.0f};

    // Static resources
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    std::unique_ptr<ppgso::Shader> shader;

public:
    glm::vec3 position{12, 0.05, -1};
    glm::vec3 scale{5, 5, 5};
    glm::vec3 color{0, 0.4490196078431373, 0.5882352941176471};

    Lake()
    {
        if (!mesh)
            mesh = std::make_unique<ppgso::Mesh>("circle.obj");
        if (!shader)
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    }

    bool update(float dTime, Scene &scene) override
    {
        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3{1.0, 0, 0});
        modelMatrix = glm::scale(modelMatrix, scale);

        return true;
    }

    void render(Scene &scene) override
    {
        // Render the object
        shader->use();
        shader->setUniform("OverallColor", color);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
        shader->setUniform("ProjectionMatrix", scene.camera->perspective);

        mesh->render();
    }
};