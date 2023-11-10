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
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

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

        m_time += dt;

        // Main Code
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);

        bool effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Gray Scale", &effect)) {
            if (effect) m_params |= GRAYSCALE_MASK;
            else m_params ^= GRAYSCALE_MASK;
        }
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect)) {
            if (effect) m_params |= COLORTINT_MASK;
            else m_params ^= COLORTINT_MASK;
        }
        if (effect) ImGui::ColorEdit3("Tint", &m_colorTint[0]);
        effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) {
            if (effect) m_params |= INVERT_MASK;
            else m_params ^= INVERT_MASK;
        }
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect)) {
            if (effect) m_params |= GRAIN_MASK;
            else m_params ^= GRAIN_MASK;
        }
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect)) {
            if (effect) m_params |= SCANLINE_MASK;
            else m_params ^= SCANLINE_MASK;
        }
        if (effect) {
            ImGui::DragFloat("Scanline Intensity", &scanlineIntensity, 0.01, 0, 1);
            ImGui::DragFloat("Scanline Spacing", &scanlineSpacing, 0.1, 0, 100);
        }
        effect = m_params & KERNEL_MASK;
        if (ImGui::Checkbox("Kernel", &effect)) {
            if (effect) m_params |= KERNEL_MASK;
            else m_params ^= KERNEL_MASK;
        }
        if (effect) ImGui::DragFloat("Offset", &kernelOffset, 0.01, 0, 400);
        ImGui::End();
        //Set Post Process Shader GUI
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program) {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("colorTint", m_colorTint);
            program->SetUniform("time", m_time);
            program->SetUniform("scanlineSpacing", scanlineSpacing);
            program->SetUniform("scanlineIntensity", scanlineIntensity);
            program->SetUniform("kernelOffset", kernelOffset);
        }

        
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // Pass 1
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();


        renderer.BeginFrame({0, 0, 0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();


        // Pass 2
        m_scene->GetActorByName("postprocess")->active = true;
        renderer.ResetViewport();

        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);
        //m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
