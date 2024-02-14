#pragma once

#include <hzrpch.h>
#include "Core/EditorEvent.h"
#include "Editor/EditorCamera.h"

#include "glm/glm.hpp"

#include "imgui.h"
#include "ImGui_Backend/ImGuizmo.h"

namespace UI 
{
	enum class Gizmo 
	{ 
		Translate	= ImGuizmo::TRANSLATE,
		Rotate		= ImGuizmo::ROTATE, 
		Scale		= ImGuizmo::SCALE,
		Bounds		= ImGuizmo::BOUNDS,
		None		= ImGuizmo::NONE
	};

	class TransformationGizmo {

	public:
		TransformationGizmo();
		~TransformationGizmo() = default;

		glm::mat4 RenderGizmo(const Editor::EditorCamera& camera, glm::mat4& target, ImVec2 size);

		bool IsUsing() { return m_IsUsing; }

		bool OnEvent(Event& e);
		Gizmo GetType() const { return m_Type; }
		void SetType(Gizmo type) { m_Type = type; }

		bool IsGlobal() const { return m_Global; }
		void SetGlobal(bool global) { m_Global = global; }
		bool ForcedSnapping() { return m_ForcedSnapping; }
		void SetForcedSnapping(bool snapping) { m_ForcedSnapping = snapping; }

	private:
		Gizmo m_Type = Gizmo::Translate;

		float m_TransformSnap = 0.5f;
		float m_RotationSnap = 10.0f;
		float m_ScaleSnap = 1.0f;

		bool m_IsUsing = false;
		bool m_Global = false;
		bool m_ForcedSnapping = false;

		float* GetSnapValues();
		bool SetSelectionContext(Events::SelectionContextChange& change);
	};
}