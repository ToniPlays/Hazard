#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {

	class MeshBindings {
	public:
		static BindMap GetBindings();
	private:
		static uint64_t Mesh_Create_Native(void* createInfo);
		static void Mesh_Destroy_Native(uint64_t assetID);
	};
}