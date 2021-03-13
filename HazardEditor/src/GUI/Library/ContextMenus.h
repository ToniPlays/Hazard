#pragma once

namespace WindowElement {
	class ContextMenus {
	public:
		static void FileContextMenu();
		static void HierarchyEntityMenu(Hazard::ECS::Scene& scene);
		static void PropertiesContextMenu(Hazard::ECS::Entity& entity);
	};
}