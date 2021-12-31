#pragma once
#include <hzrpch.h>
#include "GizmoBindings.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap GizmoBindings::GetBindings()
	{
		BindMap result;
		result.reserve(1);
		result.emplace_back("Hazard.Gizmo::Gizmo_DrawLine_Native", (void*) Gizmo_DrawLine_Native);
		return result;
	}
	void GizmoBindings::Gizmo_DrawLine_Native(glm::vec3 start, glm::vec3 end, glm::vec4 color)
	{
		Rendering::RenderCommand::DrawLine(start, end, color);
	}
}