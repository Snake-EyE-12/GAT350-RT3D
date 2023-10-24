#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>


namespace nc
{
	struct light_t
	{
		enum eType
		{
			Point = 0,
			Directional = 1,
			Spot = 2
		};

		eType type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
	};



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

		light_t m_light;
		//glm::vec3 lightPosition{0, 8, 0};
		//glm::vec3 lightDiffuse{1, 1, 1};
		glm::vec3 m_lightAmbient{0.2f, 0.2f, 0.2f};

		Transform m_transform;
		res_t<Model> m_model;
	};
}