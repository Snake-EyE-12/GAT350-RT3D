#include "RotateComponent.h"
#include "Framework/Actor.h"

namespace nc
{
	CLASS_DEFINITION(RotateComponent)
	bool RotateComponent::Initialize()
	{
		return true;
	}

	void RotateComponent::Update(float dt)
	{
		m_owner->transform.euler += euler * dt;
		glm::quat rotation = EulerToQuaternion(euler * dt);
		m_owner->transform.rotation = m_owner->transform.rotation * rotation;
	}
	void RotateComponent::SetProgram(const res_t<Program> program)
	{
		program->SetUniform("euler", euler);
	}
	void RotateComponent::ProcessGui()
	{
		ImGui::DragFloat3("Euler Direction", glm::value_ptr(euler), 0.1f);
	}
	void RotateComponent::Read(const json_t& value)
	{
		READ_DATA(value, euler);
	}
}