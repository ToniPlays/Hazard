
#include "TransformationGizmo.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "MathCore.h"
#include "HazardRenderer.h"

#include "imgui_internal.h"

namespace UI 
{
	glm::mat4 TransformationGizmo::RenderGizmo(const Editor::EditorCamera& camera, glm::mat4& target, ImVec2 size)
	{
		HZR_PROFILE_FUNCTION();
		if (m_Type == Gizmo::None || m_Type == Gizmo::Scale)
		{
			m_IsUsing = false;
			return target;
		}

		ImGuizmo::BeginFrame();

		ImGuizmo::SetOrthographic(camera.Is2DEnabled());
		ImGuizmo::SetDrawlist();

		auto deltaMatrix = glm::mat4(0.0f);

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 34, size.x, size.y);
		ImGuizmo::AllowAxisFlip(true);

		m_IsUsing = ImGuizmo::Manipulate(
			glm::value_ptr(camera.GetView()),
			glm::value_ptr(camera.GetProjection()),
			(ImGuizmo::OPERATION)m_Type, 
			m_Global ? ImGuizmo::WORLD : ImGuizmo::LOCAL, 
			glm::value_ptr(target), 
			glm::value_ptr(deltaMatrix), 
			GetSnapValues());

		return m_IsUsing ? deltaMatrix : target;
	}
	bool TransformationGizmo::OnEvent(Event& e)
	{
		return m_IsUsing;
	}
	float* TransformationGizmo::GetSnapValues()
	{
		if (Input::IsKeyDown(Key::LeftControl) || m_ForcedSnapping) 
		{
			if (m_Type == Gizmo::Translate) return hnew float[3] { m_TransformSnap, m_TransformSnap, m_TransformSnap };
			if (m_Type == Gizmo::Rotate)	return hnew float[3] { m_RotationSnap, m_RotationSnap, m_RotationSnap };
			if (m_Type == Gizmo::Scale)		return hnew float[3] { m_ScaleSnap, m_ScaleSnap, m_ScaleSnap };
		}
		return nullptr;
	}
}