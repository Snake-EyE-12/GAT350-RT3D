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
        m_model->Load("models/plane.obj");
        //m_model->Load("models/buddha.obj", glm::vec3{0}, glm::vec3{-90, 0, 0});

        m_transform.position.y = -1;
        m_light.type = light_t::eType::Point;
        m_light.direction = glm::vec3{ 0, -1, 0 };
        m_light.position = glm::vec3{ 0, 8, 0 };
        m_light.color = glm::vec3{1, 1, 1};
        m_light.intensity = 1;
        m_light.range = 5;
        m_light.innerAngle = 10.0f;
        m_light.outerAngle = 30.0f;


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
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);


        ImGui::Begin("Lighting");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(&m_light.type), types, 3);

        if(m_light.type != light_t::Directional) ImGui::DragFloat3("Position", &m_light.position[0], 0.1f);
        if (m_light.type != light_t::Point) ImGui::DragFloat3("Direction", &m_light.direction[0], 0.1f);
        ImGui::ColorEdit3("Diffuse", &m_light.color[0]);
        if (m_light.type == light_t::Spot) {
            ImGui::DragFloat("Inner Angle", &m_light.innerAngle, 1, 0, m_light.outerAngle);
            ImGui::DragFloat("Outer Angle", &m_light.outerAngle, 1, m_light.innerAngle, 90);
        }
        ImGui::DragFloat("Intensity", &m_light.intensity, 0.1f, 0, 10);
        if (m_light.type != light_t::Directional) {
            ImGui::DragFloat("Range", &m_light.range, 0.1f, 0.1f, 50);
        }
        ImGui::ColorEdit3("Ambient", &m_lightAmbient[0]);
        ImGui::End();

        material->GetProgram()->SetUniform("light.type", m_light.type);
        material->GetProgram()->SetUniform("light.position", m_light.position);
        material->GetProgram()->SetUniform("light.direction", glm::normalize(m_light.direction));
        material->GetProgram()->SetUniform("light.color", m_light.color);
        material->GetProgram()->SetUniform("light.intensity", m_light.intensity);
        material->GetProgram()->SetUniform("light.range", m_light.range);
        material->GetProgram()->SetUniform("light.innerAngle", glm::radians(m_light.innerAngle));
        material->GetProgram()->SetUniform("light.outerAngle", glm::radians(m_light.outerAngle));
        material->GetProgram()->SetUniform("ambientLight", m_lightAmbient);


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
