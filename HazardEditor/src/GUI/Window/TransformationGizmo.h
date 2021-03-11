#pragma once

#include "Hazard.h"
#include "GUI/RenderableElement.h"
#include "Core/EditorEvent.h"
#include "Editor/EditorCamera.h"

namespace WindowElement {
	enum class Gizmo { Translate, Rotate, Scale, Bounds, None };
	class TransformationGizmo : public RenderableElement {

	public:
		TransformationGizmo();
		~TransformationGizmo();

		void Init() override;
		void OnFrameBegin() override;
		void OnRender(Editor::EditorCamera camera);

		bool IsUsing() { return isUsing; }

		bool OnEvent(Event& e) override;
		Gizmo GetType() const { return this->type; }
		void SetType(Gizmo type) { this->type = type; }

		bool IsGlobal() const { return this->global; }
		void SetGlobal(bool global) { this->global = global; }
		bool ForcedSnapping() { return forcedSnapping; }
		void SetForcedSnapping(bool snapping) { forcedSnapping = snapping; }

	private:
		Hazard::ECS::Entity currentSelection;
		Gizmo type = Gizmo::Translate;

		bool isUsing = false;
		bool global = false;
		bool forcedSnapping = false;

		float* GetSnapValues();
		bool SetSelectionContext(Events::SelectionContextChange& change);
	};
}