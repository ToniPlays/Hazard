
#include "TransformationGizmo.h"
#include "imgui.h"
#include "../ImGui_Backend/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>

#include "MathCore.h"
#include "HazardRenderer.h"

namespace UI 
{
	TransformationGizmo::TransformationGizmo()
	{
	}
	void TransformationGizmo::RenderGizmo(const Editor::EditorCamera& camera, Entity& target, ImVec2 size)
	{
		ImGuizmo::BeginFrame();
		if (!target.IsValid() || m_Type == Gizmo::None) return;

		ImGuizmo::SetOrthographic(camera.Is2DEnabled());
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 17, size.x, size.y);

		auto& tc = target.GetComponent<TransformComponent>();
		auto transform = tc.GetTransformMat4();

		ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()),
			(ImGuizmo::OPERATION)m_Type, m_Global ? ImGuizmo::WORLD : ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, GetSnapValues());

		m_IsUsing = ImGuizmo::IsUsing();

		if (!m_IsUsing) return;

		//Decompose and apply modifications
		glm::vec3 position, rotation, scale;
		Math::DecomposeTransform(transform, position, rotation, scale);

		glm::vec3 deltaRotation = rotation - tc.Rotation;
		tc.Translation = position;
		tc.Rotation += deltaRotation;
		tc.Scale = scale;

	}
	bool TransformationGizmo::OnEvent(Event& e)
	{
		return false;
	}
	float* TransformationGizmo::GetSnapValues()
	{
		using namespace HazardRenderer;
		if (Input::IsKeyDown(Key::LeftControl) || m_ForcedSnapping) {

			if (m_Type == Gizmo::Translate) return new float[] { m_TransformSnap, m_TransformSnap, m_TransformSnap };
			if (m_Type == Gizmo::Rotate) return new float[] { m_RotationSnap, m_RotationSnap, m_RotationSnap };
			if (m_Type == Gizmo::Scale) return new float[] { m_ScaleSnap, m_ScaleSnap, m_ScaleSnap };
		}
		return nullptr;
	}
}