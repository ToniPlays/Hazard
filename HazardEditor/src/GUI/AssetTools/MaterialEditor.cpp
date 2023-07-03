#include "MaterialEditor.h"
#include "Hazard/Assets/AssetManager.h"
#include "Localization/Localization.h"

namespace UI
{
	using namespace Hazard;

	MaterialEditor::MaterialEditor() : Hazard::ImUI::Panel(LBL_MATERIAL_EDITOR_PANEL, false)
	{

	}

	void MaterialEditor::OnPanelRender()
	{
		if (!m_Material) return;
		/*
		const char* name = m_Material->GetPipeline()->GetSpecifications().DebugName.c_str();

		ImVec2 size = ImGui::CalcTextSize(name);
		ImUI::Shift(ImGui::GetContentRegionAvail().x * 0.5f - size.x * 0.5f, 2.0f);
		ImGui::Text("%s", name);

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 125);

		for(auto& [name, texture] : m_Material->GetTextures())
		{
			std::string text = name;
			ImUI::TextureSlot("Texture", text, texture, [&]() {
				ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
					Ref<Texture2DAsset> texture = AssetManager::GetAsset<Texture2DAsset>(handle);
					m_Material->SetTexture(text, texture);
					});
			});
		}

		for(auto& [name, param] : m_Material->GetParameters())
			DrawMaterialParam(name, param);

		ImGui::Columns();

		if(ImGui::Button("Save"))
			AssetManager::SaveAsset(m_Material);
		*/
	}
	void MaterialEditor::DrawMaterialParam(const std::string& name, const Hazard::MaterialParam& param)
	{
		using namespace HazardRenderer;
		switch (param.Type)
		{
			case ShaderDataType::Float:
			{
				float value = m_Material->Get<float>(name);
				if (ImUI::InputFloat(name.c_str(), value, 0.0f, 0.05f))
					m_Material->Set(name, value);
				break;
			}
			case ShaderDataType::Float2:
			{
				glm::vec2 value = m_Material->Get<glm::vec2>(name);
				if (ImUI::InputFloat2(name.c_str(), value))
					m_Material->Set(name, value);
				break;
			}
			case ShaderDataType::Float3:
			{
				glm::vec3 value = m_Material->Get<glm::vec3>(name);
				if (ImUI::InputFloat3(name.c_str(), value))
					m_Material->Set(name, value);
				break;
			}
			case ShaderDataType::Float4:
			{
				Color value = m_Material->Get<Color>(name);
				if (ImUI::ColorPicker(name.c_str(), name.c_str(), value))
				{
					auto c = value.ToGLM();
					m_Material->Set(name, c);
				}
				break;
			}
			case ShaderDataType::UInt:
			case ShaderDataType::Bool:
			{
				bool value = m_Material->Get<bool>(name);
				if (ImUI::Checkbox(name.c_str(), value))
					m_Material->Set(name, value);
				break;
			}
			default: break;
		}
	}
}
