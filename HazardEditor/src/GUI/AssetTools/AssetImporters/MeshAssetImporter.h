#pragma once

#include "IAssetImporter.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"


class MeshAssetImporter : public IAssetImporter
{
public:

	MeshAssetImporter();

	void Init(AssetHandle handle) override;
	virtual void Init(const std::filesystem::path& sourcePath) override;
	virtual void RenderUI() override;
	virtual bool Import() override;

private:
	void InitializeSettings();
	bool ImportFromNew();
	bool ReimportExisting();

private:
	std::filesystem::path m_SourcePath;
};