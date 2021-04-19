#pragma once

#include "../../ScriptRegistery.h"
#include <glm/glm.hpp>

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind debug calls internal
	class TransformBindings {
	public:
		static BindMap GetBindings();

		static void Transform_GetPosition_Native(uint32_t id, glm::vec3* position);
		static void Transform_SetPosition_Native(uint32_t id, glm::vec3* position);

		static void Transform_GetRotation_Native(uint32_t id, glm::vec3* rotation);
		static void Transform_SetRotation_Native(uint32_t id, glm::vec3* rotation);

		static void Transform_GetScale_Native(uint32_t id, glm::vec3* scale);
		static void Transform_SetScale_Native(uint32_t id, glm::vec3* scale);
	};
}