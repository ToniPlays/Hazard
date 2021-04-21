#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind debug calls internal
	class EntityBindings {
	public:
		static BindMap GetBindings();

		static bool IsActive(uint32_t id);
		static void SetActive(uint32_t id, bool active);

		static bool HasComponent(uint32_t id, void* type);
		static bool CreateComponent(uint32_t id, void* type);
	private:
	};
}