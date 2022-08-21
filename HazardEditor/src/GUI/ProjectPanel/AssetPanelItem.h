#pragma once

#include "Hazard/Assets/AssetManager.h"
#include "UtilityCore.h"
#include "File.h"
#include "Hazard/Rendering/Texture2D.h"

namespace UI 
{
	using namespace Hazard;

	enum AssetPanelItemFlags : uint32_t {
		AssetPanelItemFlags_StartRename = BIT(0),
		AssetPanelItemFlags_Renaming = BIT(1),
		AssetPanelItemFlags_EndRename = BIT(2)
	};

	class AssetPanelItem {
	public:
		AssetPanelItem() = default;
		AssetPanelItem(AssetHandle handle) : m_Handle(handle) { }

		void BeginRender();
		void OnRender(Ref<Texture2D> thumbnailIcon, const float& thumbnailSize);
		void EndRender();

		const AssetHandle& GetHandle() { return m_Handle; }
		const AssetMetadata& GetMetadata() { return AssetManager::GetMetadata(m_Handle); }
		std::string GetName();
		const AssetType& GetType() { return GetMetadata().Type; }

	private:
		void DrawItemName(const char* name, float edgeOffset);
		void RenameTo(const std::string& newName);
	private:
		AssetHandle m_Handle;
		uint32_t m_Flags = 0;
		std::string m_RenameValue;
	};
}