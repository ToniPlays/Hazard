#pragma once
#include <hzrpch.h>
#include "Core/EditorEvent.h"
#include "GUI/EditorWindow.h"

using namespace HazardRenderer;
using namespace Hazard::ECS;

namespace WindowElement {
	class Hierarchy : public EditorWindow {
	public:
		Hierarchy();
		~Hierarchy() = default;

		void Init() override;
		bool OnEvent(Event& e) override;

		void OnWindowRender() override;
		bool SelectionContextChange(Events::SelectionContextChange& e);
		static ECS::Entity GetSelectedEntity();

	private:
		void DrawEntity(Entity entity);
		bool KeyPressEvent(KeyPressedEvent& e);

	private:
		Entity m_SelectionContext;
		std::string m_SearchValue;
	};
}