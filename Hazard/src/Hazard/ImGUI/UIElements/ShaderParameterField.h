#pragma once
#pragma once

#include "UtilityCore.h"
#include "HazardRenderer.h"
#include <imgui_internal.h>

namespace Hazard::ImUI
{
	class ShaderParameterField
	{
	public:
		ShaderParameterField(const std::string& name, HazardRenderer::ShaderDataType type) : m_Name(name), m_Type(type) {
			m_Value.Allocate(ShaderDataTypeSize(type));
		}
		~ShaderParameterField() {
			m_Value.Release();
		}

		void Render();

		void SetValue(void* value) { 
			m_Value.Write(value, ShaderDataTypeSize(m_Type)); 
		}
		void* ValuePtr() const { return m_Value.Data; }
		bool DidChange() const { return m_DidChange; }

	private:
		std::string m_Name;
		HazardRenderer::ShaderDataType m_Type;

		bool m_DidChange = false;
		Buffer m_Value;
	};
}
