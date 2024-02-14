#pragma once

#include "IAssetImporter.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/UIElements/InputNumber.h"


class EnvironmentAssetImporter : public IAssetImporter
{
public:

	EnvironmentAssetImporter();

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
	Hazard::ImUI::Dropdown m_Resolution;
	Hazard::ImUI::InputNumber<uint32_t> m_SampleCount;
};