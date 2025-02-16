#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Math/Color.h"
#include <imgui_internal.h>

namespace Hazard::ImUI
{
	class InputColor
	{
	public:
		InputColor() = default;
		InputColor(const std::string& title) : m_Title(title) {};
		~InputColor() = default;

		void Render();

		bool DidChange() const { return m_DidChange; }
		Color GetValue() const { return m_Color; }
		void SetValue(const Color& color) { m_Color = color; }

	private:

		std::string m_Title;
		bool m_DidChange = false;
		Color m_Color;
	};
}
