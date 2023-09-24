#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Color.h"
#include <imgui_internal.h>

namespace Hazard::ImUI
{
	class TextField
	{
	public:
		TextField() = default;
		TextField(const std::string& value) : m_Value(value) {};
		~TextField() {}

		void Render();
		void Clear() { m_Value = ""; m_DidChange = true; m_DidClear = true; }
		void SetValue(const std::string& value) { m_Value = value; }
		void SetIcon(const std::string& icon) { m_Icon = icon; };
		void SetIcon(const char* icon) { m_Icon = icon; };
		void SetHint(const std::string& hint) { m_Hint = hint; };
		bool DidChange() { return m_DidChange; }
		bool DidClear() { return m_DidClear; }
		const std::string& GetValue() { return m_Value; }

	private:
		std::string m_Icon;
		std::string m_Value;
		std::string m_Hint;
		uint32_t m_Flags = 0;
		bool m_DidChange = false;
		bool m_DidClear = false;
	};
}