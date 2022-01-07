#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind Mesh calls internal
	class MeshComponentBindings {
	public:
		static BindMap GetBindings();
	private:
		static void MeshComponent_SetMesh_Native(uint32_t entityID, uint64_t meshHandle);
	};
}