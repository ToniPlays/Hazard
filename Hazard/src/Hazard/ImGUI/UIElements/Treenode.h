#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Color.h"
#include <imgui_internal.h>

#include <functional>

namespace Hazard::ImUI
{
	class Treenode
	{
	public:
		Treenode() = default;
		Treenode(const std::string& title, bool framed = false) : m_Title(title)
		{
			if (framed)
				m_Flags |= ImGuiTreeNodeFlags_Framed;
		};
		~Treenode() {}

		void Render();
		void DefaultOpen() { m_Flags |= ImGuiTreeNodeFlags_DefaultOpen; }
		void Content(std::function<void()> content) { m_ContentCallback = content; };
		void Menu(const std::string& icon, std::function<void()> menu) 
		{
			m_MenuIcon = icon; 
			m_MenuCallback = menu;
		};

	private:

		std::string m_Title;
		std::string m_MenuIcon;
		ImGuiTreeNodeFlags m_Flags = 0;
		std::function<void()> m_ContentCallback;
		std::function<void()> m_MenuCallback;
	};
}