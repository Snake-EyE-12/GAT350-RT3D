#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;

		glm::vec3 lightPosition{0, 8, 0};
		glm::vec3 lightAmbient{0.2f, 0.2f, 0.2f};
		glm::vec3 lightDiffuse{1, 1, 1};
		Transform m_transform;
		res_t<Model> m_model;
	};
}