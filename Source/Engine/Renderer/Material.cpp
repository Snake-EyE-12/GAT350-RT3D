#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Core/Core.h"

namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);

		// read the textures
		std::string albedoTextureName;
		READ_NAME_DATA(document, "albedoTexture", albedoTextureName);
		if (!albedoTextureName.empty()) {
			params |= ALBEDO_TEXTURE_MASK;
			albedoTexture = GET_RESOURCE(Texture, albedoTextureName);
		}
		std::string specularTextureName;
		READ_NAME_DATA(document, "specularTexture", specularTextureName);
		if (!specularTextureName.empty()) {
			params |= SPECULAR_TEXTURE_MASK;
			specularTexture = GET_RESOURCE(Texture, specularTextureName);
		}
		std::string emissiveTextureName;
		READ_NAME_DATA(document, "emissiveTexture", emissiveTextureName);
		if (!emissiveTextureName.empty()) {
			params |= EMISSIVE_TEXTURE_MASK;
			emissiveTexture = GET_RESOURCE(Texture, emissiveTextureName);
		}
		/*std::string normalTextureName;
		READ_NAME_DATA(document, "normalTexture", normalTextureName);
		if (!normalTextureName.empty()) {
			normalTexture = GET_RESOURCE(Texture, normalTextureName);
		}*/

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, emissive);
		//READ_DATA(document, normal);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind()
	{
		m_program->Use();
		m_program->SetUniform("material.albedo", albedo);
		m_program->SetUniform("material.specular", specular);
		m_program->SetUniform("material.emissive", emissive);
		//m_program->SetUniform("material.normal", normal);
		m_program->SetUniform("material.shininess", shininess);
		m_program->SetUniform("material.tiling", tiling);
		m_program->SetUniform("material.offset", offset);

		if (albedoTexture) {
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
		}
		if (specularTexture) {
			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
		}
		if (emissiveTexture) {
			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
		}
	}
	void Material::ProcessGUI()
	{
		ImGui::Begin("Material");
		ImGui::ColorEdit3("Albedo", &albedo[0]);
		ImGui::ColorEdit3("Specular", &specular[0]);
		ImGui::DragFloat("Shininess", &shininess, 0.1f, 2.0f, 255.0f);
		ImGui::ColorEdit3("Emissive", &emissive[0]);
		//ImGui::ColorEdit3("Normal", &normal[0] );
		ImGui::DragFloat2("Tiling", &tiling[0], 0.1f);
		ImGui::DragFloat2("Offset", &offset[0], 0.1f);
		ImGui::End();
	}
}
