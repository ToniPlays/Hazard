
#include "TransformationGizmo.h"
#include "imgui.h"
#include "GUI/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include "Core/SceneRuntimeHandler.h"

using namespace Hazard::ECS;

namespace WindowElement {
	TransformationGizmo::TransformationGizmo()
	{
		Runtime::SceneRuntimeHandler::AddRuntimeCallback([&](bool runtime) {
			currentSelection = Application::GetModule<WorldHandler>().GetCurrentWorld()->GetEntity(currentSelection);
		});
	}
	void TransformationGizmo::OnFrameBegin()
	{
		ImGuizmo::BeginFrame();
	}
	void TransformationGizmo::RenderGizmo(const Editor::EditorCamera& camera, ImVec2 size)
	{
		if (!currentSelection.IsValid() || type == Gizmo::None) return;

		ImGuizmo::SetOrthographic(camera.Is2DEnabled());
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 17, size.x, size.y);

		auto& tc = currentSelection.GetComponent<TransformComponent>();
		auto transform = tc.GetTransformMat4();

		ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()),
			(ImGuizmo::OPERATION)type, global ? ImGuizmo::WORLD : ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, GetSnapValues());

		isUsing = ImGuizmo::IsUsing();
		if (!isUsing) return;

		//Decompose and apply modifications
		glm::vec3 position, rotation, scale;
		Hazard::Math::DecomposeTransform(transform, position, rotation, scale);

		glm::vec3 deltaRotation = rotation - tc.Rotation;
		tc.Translation = position;
		tc.Rotation = rotation + deltaRotation;
		tc.Scale = scale;

	}
	bool TransformationGizmo::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(TransformationGizmo::SetSelectionContext));
	
	}
	float* TransformationGizmo::GetSnapValues()
	{
		if (Input::IsKeyDown(Key::LeftControl) || forcedSnapping) {

			if (type == Gizmo::Translate) return new float[] { transformSnap, transformSnap, transformSnap };
			if (type == Gizmo::Rotate) return new float[] { rotationSnap, rotationSnap, rotationSnap };
			if (type == Gizmo::Scale) return new float[] { scaleSnap, scaleSnap, scaleSnap };
		}
		return nullptr;
	}
	bool TransformationGizmo::SetSelectionContext(Events::SelectionContextChange& change)
	{
		currentSelection = change.GetEntity(); 
		return false;
	}
}
