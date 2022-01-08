
#include <hzrpch.h>
#include "MeshComponentBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard::Scripting::CSharp::Bindings {
	BindMap MeshComponentBindings::GetBindings()
	{
		BindMap result;
		result.emplace_back("Hazard.MeshComponent::MeshComponent_SetMesh_Native", (void*)MeshComponent_SetMesh_Native);
		return result;
	}

	void MeshComponentBindings::MeshComponent_SetMesh_Native(uint32_t entityID, uint64_t meshHandle)
	{
		using namespace Hazard::Rendering;
		auto& mc = ScriptCommand::EntityGetComponent<ECS::MeshComponent>(entityID);
		//mc.m_Mesh = AssetManager::GetAsset<Mesh>(meshHandle);
	}
}