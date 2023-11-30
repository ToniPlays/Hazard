#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "Hazard.h"
#include "HazardRenderer.h"
#include "Hazard/Rendering/Mesh/Mesh.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;
	using namespace HazardScript;

	struct ManagedVertexBufferInfo 
	{
		int Usage;
		uint32_t Size;
		//MonoArray* Data;
	};

	struct ManagedIndexBufferInfo
	{
		int Usage;
		uint32_t Size;
		//MonoArray* Data;
	};
	struct ManagedMeshInfo
	{
		UID VertexBuffer;
		UID IndexBuffer;
	};

	static uint64_t VertexBuffer_Create_Native(ManagedVertexBufferInfo* info)
	{
		void* data = nullptr;
		/*
		if (info->Data) 
		{
			data = hnew uint8_t[info->Size];
			void* dataPtr = MonoArrayUtils::GetArrayValuePtr(info->Data);
			memcpy(data, dataPtr, info->Size);
		}
		*/

		BufferLayout layout = Vertex3D::Layout();
		Ref<GPUBuffer> buffer = GPUBuffer::Create(nullptr);

		Ref<AssetPointer> asset = AssetPointer::Create(buffer, AssetType::Buffer);

		Renderer::SubmitResourceFree([dataPtr = data]() mutable {
			HZR_CORE_TRACE("Deleted C# data");
			if (!dataPtr) return;
			hdelete (uint8_t*)dataPtr;
			});
		
		AssetManager::CreateMemoryOnly(AssetType::Buffer, asset);
		return asset->GetHandle();
	}
	static void VertexBuffer_Destroy_Native(uint64_t handle) 
	{
		Ref<GPUBuffer> buffer = AssetManager::GetAsset<AssetPointer>(handle)->Value;
		buffer->DecRefCount();
	}

	static uint64_t IndexBuffer_Create_Native(ManagedIndexBufferInfo* info)
	{
		uint32_t* data = nullptr;
		/*
		if (info->Data) 
		{
			data = hnew uint32_t[info->Size * sizeof(uint32_t)];
			uint32_t* dataPtr = (uint32_t*)MonoArrayUtils::GetArrayValuePtr(info->Data);
			memcpy(data, dataPtr, info->Size * sizeof(uint32_t));
		}
		*/

		Ref<GPUBuffer> buffer = GPUBuffer::Create(nullptr);

		Ref<AssetPointer> asset = AssetPointer::Create(buffer, AssetType::Buffer);

		Renderer::SubmitResourceFree([dataPtr = data]() mutable {
			HZR_CORE_TRACE("Deleted C# data");
			if (!dataPtr) return;
			hdelete dataPtr;
			});

		AssetManager::CreateMemoryOnly(AssetType::Buffer, asset);
		return asset->GetHandle();
	}
	static void IndexBuffer_Destroy_Native(uint64_t handle)
	{
		Ref<GPUBuffer> buffer = AssetManager::GetAsset<AssetPointer>(handle)->Value;
		buffer->DecRefCount();
	}
	static uint64_t Mesh_Create_Native(ManagedMeshInfo* info)
	{
		Ref<GPUBuffer> vbo = AssetManager::GetAsset<AssetPointer>(info->VertexBuffer)->Value;
		Ref<GPUBuffer> ibo = AssetManager::GetAsset<AssetPointer>(info->IndexBuffer)->Value;

		Ref<Mesh> mesh = Ref<Mesh>::Create(vbo, ibo, nullptr);

		AssetManager::CreateMemoryOnly(AssetType::Mesh, mesh);
		return mesh->GetHandle();
	}
	static void Mesh_Destroy_Native(uint64_t handle)
	{
		Ref<Mesh> mesh = AssetManager::GetAsset<Mesh>(handle);
		mesh->DecRefCount();
	}
}
