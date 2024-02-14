
#include "Properties.h"
#include "ComponentUILibrary.h"

namespace UI
{
	Properties::Properties() : Panel("Properties") {}

	void Properties::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		if (m_SelectionContext.size() == 0) return;

		UI::ComponentMenuIfExists<TagComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<TransformComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<CameraComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<SpriteRendererComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<ScriptComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<SkyLightComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<DirectionalLightComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<PointLightComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<MeshComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<RigidbodyComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<BoxColliderComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<SphereColliderComponent>(m_SelectionContext);

		UI::ComponentMenuIfExists<Rigidbody2DComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<BoxCollider2DComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<CircleCollider2DComponent>(m_SelectionContext);

		DrawContextMenu(m_SelectionContext);

		ImUI::ShiftY(16.0f);
	}
	bool Properties::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::OnSelectionContextChange));
	}
	bool Properties::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntitites();
		return false;
	}
	void Properties::DrawContextMenu(std::vector<Entity>& e)
	{
		ImUI::ContextMenu([&]() {
			ImUI::MenuHeader("General");
			DrawAddComponentMenuIfNotExists<CameraComponent>("Camera", e);
			DrawAddComponentMenuIfNotExists<ScriptComponent>("Script", e);

			ImUI::MenuHeader("3D");
			DrawAddComponentMenuIfNotExists<MeshComponent>("Mesh component", e);
			DrawAddComponentMenuIfNotExists<RigidbodyComponent>("Rigidbody", e);
			DrawAddComponentMenuIfNotExists<BoxColliderComponent>("Box collider", e);
			DrawAddComponentMenuIfNotExists<SphereColliderComponent>("Sphere collider", e);

			ImUI::MenuHeader("2D");
			DrawAddComponentMenuIfNotExists<SpriteRendererComponent>("Sprite renderer", e);
			DrawAddComponentMenuIfNotExists<Rigidbody2DComponent>("Rigidbody 2D", e);
			DrawAddComponentMenuIfNotExists<BoxCollider2DComponent>("Box collider 2D", e);
			DrawAddComponentMenuIfNotExists<CircleCollider2DComponent>("Circle collider 2D", e);

			ImUI::MenuHeader("Lighting");
			DrawAddComponentMenuIfNotExists<SkyLightComponent>("Skylight", e);
			DrawAddComponentMenuIfNotExists<DirectionalLightComponent>("Directional light", e);
			DrawAddComponentMenuIfNotExists<PointLightComponent>("Point light", e);
		});
	}
}