#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include <vector>

namespace nc
{
	class World03 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;

		Transform m_transform;

		GLuint m_vao;
		res_t<Program> m_program;
		res_t<Texture> m_texture;

		glm::vec2 m_offset = { 1.0f, 1.0f };
		glm::vec2 m_tile = { 1.0f, 1.0f };
	};
}