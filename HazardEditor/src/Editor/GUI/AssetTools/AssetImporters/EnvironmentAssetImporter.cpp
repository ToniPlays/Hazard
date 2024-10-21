#include "EnvironmentAssetImporter.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/GUI/AssetTools/AssetPanel.h"
#include <Hazard/Rendering/Environment/EnvironmentAssetLoader.h>
#include <Hazard/ImGUI/GUIManager.h>

EnvironmentAssetImporter::EnvironmentAssetImporter()
{
	m_Resolution = Hazard::ImUI::Dropdown("Resolution", 150.0f);
	m_Resolution.SetOptions({ "512x512", "1024x1024", "2048x2048", "4096x4096", "8192x8192" });

	m_SampleCount = Hazard::ImUI::InputNumber<uint32_t>("Samples", 512);
}

void EnvironmentAssetImporter::Init(AssetHandle handle)
{
	m_SourcePath = "";
	InitializeSettings();
}

void EnvironmentAssetImporter::Init(const std::filesystem::path& sourcePath)
{
	m_SourcePath = sourcePath;
	InitializeSettings();
}

void EnvironmentAssetImporter::RenderUI()
{
	m_Resolution.Render();
	m_SampleCount.Render(150.0f);
}

bool EnvironmentAssetImporter::Import()
{
	if (!m_SourcePath.empty())
		return ImportFromNew();

	return false;
}

void EnvironmentAssetImporter::InitializeSettings()
{
	m_SampleCount.SetRange(1, 1024);
	m_SampleCount.SetValue(256);
	m_Resolution.SetSelected(3);
}

bool EnvironmentAssetImporter::ImportFromNew()
{
	using namespace Hazard;

	EnvironmentAssetLoader::CreateSettings envSettings = {
		.Resolution = (uint32_t)BIT(m_Resolution.GetSelected() + 9),
		.Samples = m_SampleCount.GetValue(),
	};

	CreateAssetSettings settings = {};
	settings.SourcePath = m_SourcePath;
	settings.Settings = &envSettings;

    Promise<Ref<EnvironmentMap>> promise = AssetManager::CreateAssetAsync<EnvironmentMap>(AssetType::EnvironmentMap, settings);

	auto& assetPanel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<UI::AssetPanel>();
	auto path = File::FindAvailableName(assetPanel.GetOpenDirectory(), File::GetNameNoExt(m_SourcePath), ".hasset");

	promise.ContinueWith([path, assetPanel](const auto& results) {
        Ref<Asset> asset = results[0];
		if (!asset) return;

		SaveAssetSettings settings = {};
		settings.TargetPath = path;
		settings.Flags = ASSET_MANAGER_SAVE_AND_UPDATE | ASSET_MANAGER_COMBINE_ASSET;

		/*AssetManager::SaveAsset(asset, settings).ContinueWith([assetPanel](const auto& results) mutable {
			assetPanel.Refresh();
		});*/
	});

    return true; //TODO: Maybe fix
}

bool EnvironmentAssetImporter::ReimportExisting()
{
	return false;
}
