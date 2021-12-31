#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings 
{
	//Bind Gizmo calls internal
	class GizmoBindings {
	public:
		static BindMap GetBindings();
		static void Gizmo_DrawLine_Native(glm::vec3 start, glm::vec3 end, glm::vec4 color);
	};
}