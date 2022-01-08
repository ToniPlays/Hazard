
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
		Ref<MeshAsset> asset = AssetManager::GetAsset<MeshAsset>(meshHandle);
		if(asset)
			mc.SourceAsset = AssetManager::GetAsset<MeshAsset>(meshHandle);
		else {
			Ref<MeshAsset> tempAsset = Ref<MeshAsset>::Create(meshHandle);
			mc.SourceAsset = tempAsset;
		}
	}
}