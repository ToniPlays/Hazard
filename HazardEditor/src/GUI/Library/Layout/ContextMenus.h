#pragma once

namespace WindowElement {
	class ContextMenus {
	public:
		static void FileContextMenu();
		static void HierarchyEntityMenu(Hazard::ECS::World& world, void(*entityAdded)(Hazard::ECS::Entity));
		static void PropertiesContextMenu(Hazard::ECS::Entity& entity);
	};
}