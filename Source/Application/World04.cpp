#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

//#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {

        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/sphere.obj");



        //vertex data
        float vertexData[] = {
           -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
           -0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };



        //m_vertexBuffer = std::make_shared<VertexBuffer>();
        //m_vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 4, vertexData);
        //m_vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);                  // position 
        //m_vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(float));  // color 
        //m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();



        m_transform.rotation.z += 18 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * m_speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * m_speed : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) ? +dt * m_speed : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LSHIFT) ? -dt * m_speed : 0;
        m_time += dt;

        auto material = m_model->GetMaterial();

        material->ProcessGUI();
        material->Bind();

        // model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view
        glm::mat4 view = glm::lookAt(glm::vec3{0, 0, 3}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        material->GetProgram()->SetUniform("view", view);

        //projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);


        ImGui::Begin("Lighting");
        ImGui::ColorEdit3("Ambient", &lightAmbient[0]);
        ImGui::ColorEdit3("Diffuse", &lightDiffuse[0]);
        ImGui::DragFloat3("Position", &lightPosition[0], 0.1f);
        ImGui::End();

        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("light.acolor", lightAmbient);
        material->GetProgram()->SetUniform("light.dcolor", lightDiffuse);


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        // render
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_model->Draw(GL_TRIANGLES);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
