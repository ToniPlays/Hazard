#pragma once
#include "GUI/Window/Inspect/FileView.h"
namespace WindowElement {
	class ContextMenus {
	public:
		static void FileContextMenu(FileView& view);
		static void HierarchyEntityMenu(Hazard::ECS::World& world, void(*entityAdded)(Hazard::ECS::Entity));
		static void PropertiesContextMenu(Hazard::ECS::Entity& entity);
	};
}