#include "MeshAssetImporter.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/GUI/AssetTools/AssetPanel.h"
#include <Hazard/RenderContext/ImageAssetLoader.h>
#include <Hazard/ImGUI/GUIManager.h>
#include "Hazard/ImGUI/StyleManager.h"

MeshAssetImporter::MeshAssetImporter()
{
	const Hazard::ImUI::Style& style = Hazard::ImUI::StyleManager::GetCurrent();

	m_TransformTreenode = Hazard::ImUI::Treenode("Transform", 125.0f);
	m_MaterialTreenode = Hazard::ImUI::Treenode("Materials", 125.0f);
	m_AnimationTreenode = Hazard::ImUI::Treenode("Animations", 125.0f);

	m_TransformTreenode.Content([this]() {
		m_Translation.Render();
        Hazard::ImUI::ShiftY(4.0f);
		m_Rotation.Render();
        Hazard::ImUI::ShiftY(4.0f);
	});

	m_MaterialTreenode.Content([this]() {

		using namespace Hazard;
		bool import = m_Settings.Flags & MESH_CREATE_INCLUDE_MATERIALS;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 125.0f);

		if (Hazard::ImUI::Checkbox("Import materials", import, false))
			m_Settings.Flags ^= MESH_CREATE_INCLUDE_MATERIALS;

		ImGui::Columns();
	});

	m_AnimationTreenode.Content([]() {});

	m_Translation.ConfigureField(0, "X", style.Colors.AxisX);
	m_Translation.ConfigureField(1, "Y", style.Colors.AxisY);
	m_Translation.ConfigureField(2, "Z", style.Colors.AxisZ);

	m_Rotation.ConfigureField(0, "X", style.Colors.AxisX);
	m_Rotation.ConfigureField(1, "Y", style.Colors.AxisY);
	m_Rotation.ConfigureField(2, "Z", style.Colors.AxisZ);
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
	m_TransformTreenode.Render();
	m_MaterialTreenode.Render();
	m_AnimationTreenode.Render();
}

bool MeshAssetImporter::Import()
{
	if (!m_SourcePath.empty())
		return ImportFromNew();

	return false;
}

void MeshAssetImporter::InitializeSettings()
{
	using namespace Hazard;
	m_Settings = MeshAssetLoader::CreateSettings();
	m_Settings.Flags |= MESH_CREATE_INCLUDE_MATERIALS;
}

bool MeshAssetImporter::ImportFromNew()
{
	using namespace Hazard;

	CreateAssetSettings settings = {};
	settings.SourcePath = m_SourcePath;
	settings.Settings = &m_Settings;

	auto& assetPanel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<UI::AssetPanel>();
	std::filesystem::path rootPath = Directory::FindAvailableName(assetPanel.GetOpenDirectory() / File::GetNameNoExt(m_SourcePath), File::GetNameNoExt(m_SourcePath));
	std::filesystem::path path = rootPath / (File::GetNameNoExt(m_SourcePath) + ".hasset");
	m_Settings.MaterialPath = rootPath / "Materials";
	m_Settings.TexturePath = rootPath / "Textures";

    Promise<Ref<Mesh>> promise = AssetManager::CreateAssetAsync<Mesh>(AssetType::Mesh, settings);
	promise.ContinueWith([path, assetPanel](const auto& results) {
        Ref<Asset> asset = results[0];
		if (!asset) return;

		SaveAssetSettings settings = {};
		settings.TargetPath = path;
		settings.Flags = ASSET_MANAGER_SAVE_AND_UPDATE | ASSET_MANAGER_COMBINE_ASSET;

		/*AssetManager::SaveAsset(asset, settings).ContinueWith([assetPanel](const auto& results) mutable {
			Application::Get().SubmitMainThread([assetPanel]() mutable {
				assetPanel.Refresh();
			});
		});*/
	});

    return true; //TODO: maybe fix this
}

bool MeshAssetImporter::ReimportExisting()
{
	return false;
}
