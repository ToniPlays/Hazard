#pragma once

#include "GUIRenderable.h"
#include "Utility/StringUtil.h"
#include <functional>

#include "imgui.h"

namespace UI
{
	struct MenuItemElement {
		std::string Name;
		std::function<void()> OnClicked;

		std::vector<MenuItemElement> m_SubMenus;

		MenuItemElement& GetSubMenu(const std::string& name) {
			for (auto& item : m_SubMenus) {
				if (item.Name == name) return item;
			}
			//Does not exist yet
			MenuItemElement& item = m_SubMenus.emplace_back();
			item.Name = name;
			item.OnClicked = nullptr;
			return item;
		}
		MenuItemElement& GetSubMenuFromPath(std::vector<std::string> path) {
			for (auto& item : m_SubMenus) {
				if (item.Name == path[0]) {
					if (path.size() <= 1) return item;
					else return item.GetSubMenuFromPath({ path.begin() + 1, path.end() });
				}
			}
			MenuItemElement& item = m_SubMenus.emplace_back();
			item.Name = path[0];

			if (path.size() <= 1) return item;
			else return item.GetSubMenuFromPath({ path.begin() + 1, path.end() });
		}
	};


	class MenuBar : public GUIRenderable {
	public:
		void Render() override {
			ImGui::BeginMainMenuBar();

			for (auto& item : m_MenuItems) {
				if (ImGui::BeginMenu(item.Name.c_str(), true)) {
					RenderSubMenu(item);
					ImGui::EndMenu();
				}
			}

			ImGui::EndMainMenuBar();
		};
		void AddMenuItem(const std::string& name, const std::function<void()> onClick = nullptr)
		{
			std::vector<std::string> path = StringUtil::SplitString(name, '/');

			uint32_t index = 0;
			MenuItemElement& target = GetMenu(name);
			target.OnClicked = onClick;
		}
		void ClearMenuBar() {
			m_MenuItems.clear();
		}
		MenuItemElement& GetMenu(const std::string& name) {

			std::vector<std::string> path = StringUtil::SplitString(name, '/');

			MenuItemElement& heading = FindMenu(path[0]);
			if (path.size() <= 1) return heading;

			return heading.GetSubMenuFromPath({ path.begin() + 1, path.end() });
		}
	private:

		MenuItemElement& FindMenu(const std::string& name) {
			for (auto& item : m_MenuItems) {
				if (item.Name == name) return item;
			}
			//Does not exist yet
			MenuItemElement& item = m_MenuItems.emplace_back();
			item.Name = name;
			item.OnClicked = nullptr;
			return item;

		}

		void RenderSubMenu(const MenuItemElement& item) {
			for (auto& subItem : item.m_SubMenus) {

				if (subItem.m_SubMenus.size()) {
					if (ImGui::BeginMenu(subItem.Name.c_str())) {
						RenderSubMenu(subItem);
						ImGui::EndMenu();
					}
					continue;
				}
				if (ImGui::MenuItem(subItem.Name.c_str())) {
					if (subItem.OnClicked) {
						subItem.OnClicked();
					}
				}
			}
		}

	private:
		std::vector<MenuItemElement> m_MenuItems;
	};
}