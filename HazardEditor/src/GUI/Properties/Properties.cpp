
#include "Properties.h"
#include "ComponentUILibrary.h"

namespace UI
{
	Properties::Properties() : Panel("Properties") {}

	void Properties::OnPanelRender()
	{
		if (!m_SelectionContext.IsValid()) return;

		UI::ComponentMenuIfExists<TagComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<TransformComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<CameraComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<SpriteRendererComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<BatchComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<ScriptComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<SkyLightComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<DirectionalLightComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<PointLightComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<MeshComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<Rigidbody2DComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<BoxCollider2DComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<CircleCollider2DComponent>(m_SelectionContext);

		{
			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
			ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			ImUI::Shift(ImGui::GetContentRegionAvailWidth() / 2.0f - 65.0f, 50.0f);

			if (ImGui::Button("Add component", { 130, 40 })) 
			{
				
			}
		}
		DrawContextMenu(m_SelectionContext);
	}
	bool Properties::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::OnSelectionContextChange));
	}
	bool Properties::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntity();
		return false;
	}
	void Properties::DrawContextMenu(Entity& e)
	{
		ImUI::ContextMenu([&]() {
			ImUI::Submenu("General", [&]() {
				DrawAddComponentMenuIfNotExists<ScriptComponent>("Script", e);
				DrawAddComponentMenuIfNotExists<CameraComponent>("Camera", e);
				});
			ImUI::Submenu("3D", [&]() {
				DrawAddComponentMenuIfNotExists<MeshComponent>("Mesh component", e);
				});
			ImUI::Submenu("2D", [&]() {
				DrawAddComponentMenuIfNotExists<SpriteRendererComponent>("Sprite renderer", e);
				DrawAddComponentMenuIfNotExists<BatchComponent>("Batch renderer", e);

				DrawAddComponentMenuIfNotExists<Rigidbody2DComponent>("Rigidbody 2D", e);
				DrawAddComponentMenuIfNotExists<BoxCollider2DComponent>("Box collider 2D", e);
				DrawAddComponentMenuIfNotExists<CircleCollider2DComponent>("Circle collider 2D", e);
				});
			ImUI::Submenu("Lighting", [&]() {
				DrawAddComponentMenuIfNotExists<SkyLightComponent>("Skylight", e);
				DrawAddComponentMenuIfNotExists<DirectionalLightComponent>("Directional light", e);
				DrawAddComponentMenuIfNotExists<PointLightComponent>("Point light", e);
				});
			});
	}
}