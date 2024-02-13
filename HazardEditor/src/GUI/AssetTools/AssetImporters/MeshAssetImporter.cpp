#include "MeshAssetImporter.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "GUI/AssetTools/AssetPanel.h"
#include <Hazard/RenderContext/ImageAssetLoader.h>
#include <Hazard/ImGUI/GUIManager.h>

MeshAssetImporter::MeshAssetImporter()
{

}

void MeshAssetImporter::Init(AssetHandle handle)
{
	m_SourcePath = "";
	InitializeSettings();
}

void MeshAssetImporter::Init(const std::filesystem::path& sourcePath)
{
	m_SourcePath = sourcePath;
	InitializeSettings();
}

void MeshAssetImporter::RenderUI()
{

}

bool MeshAssetImporter::Import()
{
	if (!m_SourcePath.empty())
		return ImportFromNew();

	return false;
}

void MeshAssetImporter::InitializeSettings()
{

}

bool MeshAssetImporter::ImportFromNew()
{
	using namespace Hazard;

	CreateAssetSettings settings = {};
	settings.SourcePath = m_SourcePath;

	JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Mesh, settings);

	auto assetPanel = Application::Get().GetModule<Hazard::GUIManager>().GetRenderable<UI::AssetPanel>();
	auto path = File::FindAvailableName(assetPanel->GetOpenDirectory(), File::GetNameNoExt(m_SourcePath), "hasset");

	promise.Then([path](JobGraph& graph) {
		Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
		if (!asset) return;

		SaveAssetSettings settings = {};
		settings.TargetPath = path;
		settings.Flags = ASSET_MANAGER_SAVE_AND_UPDATE | ASSET_MANAGER_COMBINE_ASSET;

		AssetManager::SaveAsset(asset, settings);
	});

	return promise.Valid();
}

bool MeshAssetImporter::ReimportExisting()
{
	return false;
}
