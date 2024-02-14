#include "ImageAssetImporter.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/GUI/AssetTools/AssetPanel.h"
#include <Hazard/RenderContext/ImageAssetLoader.h>
#include <Hazard/ImGUI/GUIManager.h>

ImageAssetImporter::ImageAssetImporter()
{
	using namespace Hazard;

	m_ResolutionDropdown = ImUI::Dropdown("Maximum resolution", 150.0f);
	m_AdvancedTreenode = ImUI::Treenode("Advanced settings", true);
}

void ImageAssetImporter::Init(AssetHandle handle)
{
	using namespace Hazard;

	m_SourcePath = "";
	m_Handle = handle;
	InitializeSettings();
}
void ImageAssetImporter::Init(const std::filesystem::path& sourceFile)
{
	m_Handle = INVALID_ASSET_HANDLE;
	m_SourcePath = sourceFile;
	InitializeSettings();
}

void ImageAssetImporter::RenderUI()
{
	using namespace Hazard;
	m_ResolutionDropdown.Render();
	ImUI::ShiftY(8.0f);
	m_AdvancedTreenode.Content([]() {
		ImGui::Text("Filters");
		ImUI::ShiftY(8.0f);
	});
	m_AdvancedTreenode.Render();
}

bool ImageAssetImporter::Import()
{
	if (m_Handle == INVALID_ASSET_HANDLE)
		return ImportFromNew();
	return ReimportExisting();
}

void ImageAssetImporter::InitializeSettings()
{
	m_ResolutionDropdown.SetOptions({ "64x64", "128x128", "256x256", "512x512", "1024x1024", "2048x2048", "4096x4096", "8192x8192" });
	m_ResolutionDropdown.SetSelected(6);
}

bool ImageAssetImporter::ImportFromNew()
{
	using namespace Hazard;
	ImageAssetLoader::CreateSettings imageSettings = {};
	imageSettings.Resolution = BIT(m_ResolutionDropdown.GetSelected() + 6);

	CreateAssetSettings settings = {};
	settings.SourcePath = m_SourcePath;
	settings.Settings = &imageSettings;

	JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Image, settings);

	auto& assetPanel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<UI::AssetPanel>();
	auto path = File::FindAvailableName(assetPanel.GetOpenDirectory(), File::GetNameNoExt(m_SourcePath), "hasset");

	promise.Then([path, assetPanel](JobGraph& graph) {
		Ref<Asset> asset = graph.GetResult<Ref<Asset>>();

		SaveAssetSettings settings = {};
		settings.TargetPath = path;
		settings.Flags |= ASSET_MANAGER_COMBINE_ASSET;

		AssetManager::SaveAsset(asset, settings).Then([assetPanel](JobGraph&) mutable { 
			assetPanel.Refresh();
		});
	});

	return promise.Valid();
}

bool ImageAssetImporter::ReimportExisting()
{
	using namespace Hazard;

	Ref<Texture2DAsset> asset = AssetManager::GetAsset<Texture2DAsset>(m_Handle);
	asset->SetMaxResolution(BIT(m_ResolutionDropdown.GetSelected() + 6));
	asset->SetMaxMipLevels(1);

	SaveAssetSettings settings = {};
	settings.Flags = ASSET_MANAGER_SAVE_AND_UPDATE | ASSET_MANAGER_COMBINE_ASSET;

	JobPromise promise = AssetManager::SaveAsset(asset, settings);
	promise.Then([handle = asset->GetHandle()](JobGraph&) {
		AssetManager::Reload(handle).Wait();
	});

	return promise.Valid();
}
