#pragma once

#include "Hazard.h"
#include "GUI/RenderableElement.h"
#include "Core/EditorEvent.h"
#include "Editor/EditorCamera.h"

namespace WindowElement {
	enum Gizmo { Translate, Rotate, Scale, Bounds, None };
	class TransformationGizmo : public RenderableElement {


	public:
		TransformationGizmo();
		~TransformationGizmo();

		void Init() override;
		void OnFrameBegin() override;
		void OnRender(Editor::EditorCamera camera);

		bool IsUsing() { return isUsing; }

		bool OnEvent(Event& e) override;
		void SetType(Gizmo type) { this->type = type; }

	private:
		Hazard::ECS::Entity currentSelection;
		Gizmo type = Gizmo::Translate;

		bool isUsing = false;

		float* GetSnapValues();
		bool SetSelectionContext(Events::SelectionContextChange& change);
	};
}