#pragma once

#include "Hazard/Assets/AssetManager.h"
#include "UtilityCore.h"
#include "File.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "HazardRendererCore.h"

namespace UI 
{
	enum AssetPanelItemFlags : uint32_t 
	{
		AssetPanelItemFlags_StartRename = BIT(0),
		AssetPanelItemFlags_Renaming = BIT(1),
		AssetPanelItemFlags_EndRename = BIT(2)
	};

	class AssetPanelItem
    {
	public:
		AssetPanelItem() = default;
		AssetPanelItem(Hazard::AssetHandle handle, const std::filesystem::path& sourcePath) : m_Handle(handle), m_SourcePath(sourcePath) { }

		void BeginRender();
		void OnRender(Ref<Hazard::Texture2DAsset> thumbnailIcon, Ref<HazardRenderer::Sampler> sampler, const float& thumbnailSize);
		void EndRender();

		bool IsFolder() { return File::IsDirectory(m_SourcePath); }
		const Hazard::AssetHandle& GetHandle() { return m_Handle; }
		const Hazard::AssetMetadata& GetMetadata() { return Hazard::AssetManager::GetMetadata(m_Handle); }
		std::string GetName();
		const AssetType& GetType() { return GetMetadata().Type; }

	private:
		void DrawItemName(const char* name, float edgeOffset);
		void RenameTo(const std::string& newName);
        
        void OnItemClicked();
        
	private:
		Hazard::AssetHandle m_Handle;
		uint32_t m_Flags = 0;
		std::string m_RenameValue;
		std::filesystem::path m_SourcePath;
	};
}
