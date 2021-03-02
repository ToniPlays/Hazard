#pragma once

#include <hzreditor.h>
#include "TransformationGizmo.h"
#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace Hazard::ECS;

namespace WindowElement {

	TransformationGizmo::TransformationGizmo()
	{

	}
	TransformationGizmo::~TransformationGizmo()
	{

	}
	void TransformationGizmo::Init()
	{

	}
	void TransformationGizmo::OnFrameBegin()
	{
		ImGuizmo::BeginFrame();
	}
	void TransformationGizmo::OnRender(Editor::EditorCamera camera)
	{
		if (!currentSelection.IsValid() || type == Gizmo::None) return;

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		auto& tc = currentSelection.GetComponent<TransformComponent>();
		auto transform = tc.GetTransformMat4();

		ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()),
			(ImGuizmo::OPERATION)type, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, GetSnapValues());

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
		
		if (Input::IsKeyDown(Key::LeftControl)) {

			if (type == Gizmo::Translate) return new float[] { 0.5f, 0.5f, 0.5f };
			if (type == Gizmo::Rotate) return new float[] { 10.0f, 10.0f, 10.0f };
			if (type == Gizmo::Scale) return new float[] { 1.0f, 1.0f, 1.0f };
		}
		return nullptr;
	}
	bool TransformationGizmo::SetSelectionContext(Events::SelectionContextChange& change)
	{
		currentSelection = change.GetEntity(); 
		return false;
	}
}