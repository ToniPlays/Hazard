#pragma once

#include "IAssetImporter.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/UIElements/InputFloat.h"


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
	Hazard::ImUI::Treenode m_TransformTreenode;
	Hazard::ImUI::Treenode m_MaterialTreenode;
	Hazard::ImUI::Treenode m_AnimationTreenode;

	Hazard::ImUI::InputFloat m_Translation = Hazard::ImUI::InputFloat("Translation", 3);
	Hazard::ImUI::InputFloat m_Rotation = Hazard::ImUI::InputFloat("Rotation", 3);

	std::filesystem::path m_SourcePath;
};