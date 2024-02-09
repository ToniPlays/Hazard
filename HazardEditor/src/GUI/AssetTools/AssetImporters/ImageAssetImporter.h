#pragma once

#include "IAssetImporter.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"


class ImageAssetImporter : public IAssetImporter
{
public:

	ImageAssetImporter();

	void Init(AssetHandle handle) override;
	virtual void Init(const std::filesystem::path& sourcePath) override;
	virtual void RenderUI() override;
	virtual bool Import() override;

private:
	void InitializeSettings();

private:

	AssetHandle m_Handle;
	std::filesystem::path m_SourcePath;

	Hazard::ImUI::Dropdown m_ResolutionDropdown;
	Hazard::ImUI::Treenode m_AdvancedTreenode;
};