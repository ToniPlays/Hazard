#pragma once

namespace WindowElement {
	class ContextMenus {
	public:
		static void FileContextMenu();
		static void HierarchyEntityMenu(Hazard::ECS::Scene& scene, void(*entityAdded)(Hazard::ECS::Entity));
		static void PropertiesContextMenu(Hazard::ECS::Entity& entity);
	};
}