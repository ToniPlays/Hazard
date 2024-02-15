#include "MeshAssetImporter.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Editor/GUI/AssetTools/AssetPanel.h"
#include <Hazard/RenderContext/ImageAssetLoader.h>
#include <Hazard/ImGUI/GUIManager.h>

MeshAssetImporter::MeshAssetImporter()
{
	using namespace Hazard::ImUI;

	const Style& style = StyleManager::GetCurrent();

	m_TransformTreenode = Treenode("Transform", 125.0f);
	m_MaterialTreenode = Treenode("Materials", 125.0f);
	m_AnimationTreenode = Treenode("Animations", 125.0f);

	m_TransformTreenode.Content([this]() {
		m_Translation.Render();
		ShiftY(4.0f);
		m_Rotation.Render();
		ShiftY(4.0f);
	});

	m_MaterialTreenode.Content([this]() {

		using namespace Hazard;
		bool import = m_Settings.Flags & MESH_CREATE_INCLUDE_MATERIALS;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 125.0f);

		if (Checkbox("Import materials", import, false))
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
	

	JobPromise promise = AssetManager::CreateAssetAsync(AssetType::Mesh, settings);

	auto& assetPanel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<UI::AssetPanel>();
	auto path = File::FindAvailableName(assetPanel.GetOpenDirectory() / File::GetNameNoExt(m_SourcePath), File::GetNameNoExt(m_SourcePath), "hasset");
	m_Settings.MaterialPath = path / "Materials";

	promise.Then([path, assetPanel](JobGraph& graph) {
		Ref<Asset> asset = graph.GetResult<Ref<Asset>>();
		if (!asset) return;

		SaveAssetSettings settings = {};
		settings.TargetPath = path;
		settings.Flags = ASSET_MANAGER_SAVE_AND_UPDATE | ASSET_MANAGER_COMBINE_ASSET;

		AssetManager::SaveAsset(asset, settings).Then([assetPanel](JobGraph&) mutable {
			Application::Get().SubmitMainThread([assetPanel]() mutable {
				assetPanel.Refresh();
			});
		});
	});

	return promise.Valid();
}

bool MeshAssetImporter::ReimportExisting()
{
	return false;
}
