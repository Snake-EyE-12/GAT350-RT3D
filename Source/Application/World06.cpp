#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>
#include <Core/StringUtils.h>


namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        /*{
            auto actor = CREATE_CLASS(Actor);
            actor->name = "camera1";
            actor->transform.position = glm::vec3{ 0, 0, -3 };
            actor->transform.rotation = glm::radians(glm::vec3{ 0, 0, 0 });

            auto cameraComponent = CREATE_CLASS(CameraComponent);
            cameraComponent->SetPerspective(70.0f, ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
            actor->AddComponent(std::move(cameraComponent));
            
            auto cameraController = CREATE_CLASS(CameraController);
            cameraController->speed = 5;
            cameraController->sensitivity = 0.5f;
            cameraController->m_owner = actor.get();
            cameraController->Initialize();
            actor->AddComponent(std::move(cameraController));

            m_scene->Add(std::move(actor));
        }*/
        /*for (int i = 0; i < 10; i++) {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->name = StringUtils::CreateUnique("tree");
            actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
            actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), 1 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }*/


        /*{
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->model = std::make_shared<Model>();
            modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }
        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor2";
            actor->transform.position = glm::vec3{ 5, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->model = std::make_shared<Model>();
            modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }*/

        /*{
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }*/
        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        //auto actor = m_scene->GetActorByName<Actor>("actor1");

        /*actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? dt * m_speed : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? -dt * m_speed : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? +dt * m_speed : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt * m_speed : 0;
        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) ? +dt * m_speed : 0;
        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LSHIFT) ? -dt * m_speed : 0;*/
        m_time += dt;

        //auto material = actor->GetComponent<ModelComponent>()->material;

        //material->ProcessGUI();
        //material->Bind();

        /*material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use(); 
            program->SetUniform("ior", m_refraction);
            ImGui::End();

        }*/
        
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // Pass 1
        m_scene->GetActorByName("cube")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();


        renderer.BeginFrame({0, 0, 1});
        m_scene->Draw(renderer);

        framebuffer->Unbind();


        // Pass 2
        m_scene->GetActorByName("cube")->active = true;
        renderer.ResetViewport();

        renderer.BeginFrame();
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
