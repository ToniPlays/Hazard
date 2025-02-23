#include <hzrpch.h>

#include "ShaderParameterField.h"
#include "../StyleManager.h"

#include "Hazard/ImGUI/UIElements/InputFloat.h"
#include "Hazard/ImGUI/UIElements/InputColor.h"
#include <Hazard/ImGUI/UILibrary.h>

namespace Hazard::ImUI
{
	void ShaderParameterField::Render()
	{
		m_DidChange = false;

		const Style& style = StyleManager::GetCurrent();

		switch (m_Type)
		{
			case HazardRenderer::ShaderDataType::Float:
			{
				ImGui::PushID(m_Name.c_str());
				float value = m_Value.Read<float>();

				if (SliderFloat(m_Name.c_str(), value, 0.0f, 0.0f, 1.0f))
				{
					m_DidChange = true;
					m_Value.Write(&value, sizeof(float));
				}

				ImGui::PopID();
				break;
			}
			case HazardRenderer::ShaderDataType::Float2:
			case HazardRenderer::ShaderDataType::Float3:
			{
				const char* txt[3] = { "X", "Y", "Z" };
				const ImVec4 col[3] = { style.Colors.AxisX, style.Colors.AxisY, style.Colors.AxisZ };
				uint32_t count = HazardRenderer::ComponentCount(m_Type);
				InputFloat input(m_Name, count);

				for (uint32_t i = 0; i < count; i++)
				{
					input.ConfigureField(i, txt[i], col[i]);
					input.SetFieldValue(i, m_Value.Read<float>(i * sizeof(float)), 1.0f);
				}

				input.Render();
				m_DidChange = input.DidAnyChange();

				if (m_DidChange)
				{
					for (uint32_t i = 0; i < count; i++)
					{
						float value = input.GetValue(i);
						m_Value.Write(&value, sizeof(float), i * sizeof(float));
					}
				}

				break;
			}
			case HazardRenderer::ShaderDataType::Float4:
			{
				Color& color = *(Color*)m_Value.Data;
				InputColor input(m_Name);
				input.SetValue(color);
				input.Render();

				m_DidChange = input.DidChange();
				if (m_DidChange)
				{
					Color v = input.GetValue();
					m_Value.Write(&v, sizeof(Color));
				}

				break;
			}
			case HazardRenderer::ShaderDataType::Int:
				break;
			case HazardRenderer::ShaderDataType::Int2:
				break;
			case HazardRenderer::ShaderDataType::Int3:
				break;
			case HazardRenderer::ShaderDataType::Int4:
				break;
			case HazardRenderer::ShaderDataType::UInt:
				break;
			case HazardRenderer::ShaderDataType::UInt2:
				break;
			case HazardRenderer::ShaderDataType::UInt3:
				break;
			case HazardRenderer::ShaderDataType::UInt4:
				break;
			case HazardRenderer::ShaderDataType::Bool:
			{
				ImGui::PushID(m_Name.c_str());
				bool value = m_Value.Read<bool>();

				if (Checkbox("##val", value))
				{
					m_DidChange = true;
					m_Value.Write(&value, sizeof(bool));
				}

				ImGui::PopID();
				break;
			}
			default:
				break;
		}
	}
}