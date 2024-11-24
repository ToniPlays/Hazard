#pragma once

#include "Hazard/Assets/AssetManager.h"
#include "UtilityCore.h"
#include "Filesystem/File.h"
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
		AssetPanelItem(const std::filesystem::path& path) : m_SourcePath(path) { }

		void BeginRender();
		void OnRender(Ref<Hazard::Texture2DAsset> thumbnailIcon, Ref<HazardRenderer::Sampler> sampler, const float& thumbnailSize);
		void EndRender();

        const std::filesystem::path& GetPath() const { return m_SourcePath; }
		std::string GetName();

	private:
		void DrawItemName(const char* name, float edgeOffset);
		void RenameTo(const std::string& newName);
        
        void OnItemClicked();
        void OnItemDoubleClicked();
        
	private:
		uint32_t m_Flags = 0;
		std::string m_RenameValue;
		std::filesystem::path m_SourcePath;
	};
}
