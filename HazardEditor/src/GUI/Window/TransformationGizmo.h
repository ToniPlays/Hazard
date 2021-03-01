#pragma once

#include "Hazard.h"
#include "GUI/RenderableElement.h"
#include "Core/EditorEvent.h"

namespace WindowElement {
	enum Gizmo { Translate, Rotate, Scale, Bounds, None };
	class TransformationGizmo : public RenderableElement {


	public:
		TransformationGizmo();
		~TransformationGizmo();

		void Init() override;
		void OnFrameBegin() override;
		void OnRender() override;

		bool OnEvent(Event& e) override;
		void SetType(Gizmo type) { this->type = type; }

	private:
		Hazard::ECS::Entity currentSelection;
		Gizmo type = Gizmo::Translate;

		float* GetSnapValues();
		bool SetSelectionContext(Events::SelectionContextChange& change);
	};
}