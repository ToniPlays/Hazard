#include "AssetImporterPanel.h"
#include "Core/EditorAssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Core/EditorAssetPackBuilder.h"
#include <spdlog/fmt/fmt.h>

#include "Core/GUIManager.h"
#include "GUI/ProjectPanel/AssetPanel.h"

namespace UI
{
	using namespace Hazard;
	AssetImporterPanel::AssetImporterPanel() : Hazard::ImUI::Modal("Asset importer", { 500, 600 })
	{

	}
	void AssetImporterPanel::Open(const std::filesystem::path& path)
	{
		m_AssetHandle = AssetHandle();
		m_CurrentFilePath = path;
		m_Open = true;
		m_IsImportingNew = true;

		m_AssetType = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));
		m_Title = fmt::format("{} \"{}\" import settings", Hazard::Utils::AssetTypeToString(m_AssetType), File::GetNameNoExt(path));

		InitializeData();
	}
	void AssetImporterPanel::OpenExisting(const std::filesystem::path& path, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		m_AssetType = metadata.Type;
		m_Open = true;
		m_IsImportingNew = false;

		m_CurrentFilePath = path;
		m_AssetHandle = handle;

		m_Title = fmt::format("{} \"{}\" import settings", Hazard::Utils::AssetTypeToString(m_AssetType), File::GetNameNoExt(metadata.Key));
		InitializeData();
	}
	void AssetImporterPanel::OnPanelRender()
	{
		switch (m_AssetType)
		{
			case AssetType::Image:
				DrawImageImportSettings();
				break;
			case AssetType::EnvironmentMap:
				DrawEnvironmentMapImportSettings();
				break;
			case AssetType::Mesh:
				DrawMeshImportSettings();
				break;
		}

		DrawBottomBar();
	}
	void AssetImporterPanel::DrawBottomBar()
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImVec2 buttonPos = ImGui::GetWindowSize();
		buttonPos.x -= 104.0f;
		buttonPos.y -= 40.0f;
		ImGui::SetCursorPos(buttonPos);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);

		if (ImGui::Button(m_IsImportingNew ? "Import" : "Reimport", { 100, 32 }))
		{
			m_Import = true;
		}
	}
	void AssetImporterPanel::DrawImageImportSettings()
	{
		using namespace HazardRenderer;
		ImageImportSettings info = m_ImportDataBuffer.Read<ImageImportSettings>();

		ImUI::ShiftY(4);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 200);
		{
			ImGui::Text("Texture wrap mode");
			ImGui::NextColumn();

			std::string wrapMode[] = { "Repeat", "Repeat mirrored", "Clamp to border", "Clamp to edge" };
			uint32_t selected = (uint32_t)info.Wrapping;

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::Combo("##wrapMode", wrapMode, 4, selected))
			{
				info.Wrapping = (ImageWrap)selected;
			}
		}
		ImGui::NextColumn();
		std::string filter[] = { "Linear", "Nearest", "Linear mip", "Nearest mip" };
		{
			ImGui::Text("Texture min filter");
			ImGui::NextColumn();

			uint32_t selected = (uint32_t)info.MinFilter;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::Combo("##minfilter", filter, 4, selected))
				info.MinFilter = (FilterMode)selected;
		}
		ImGui::NextColumn();
		{
			ImGui::Text("Texture mag filter");
			ImGui::NextColumn();

			uint32_t selected = (uint32_t)info.MagFilter;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::Combo("##magfilter", filter, 4, selected))
				info.MagFilter = (FilterMode)selected;
		}
		ImGui::NextColumn();
		ImUI::Checkbox("Generate mipmaps", info.GenerateMips);
		ImUI::Checkbox("Flip vertically", info.FlipOnLoad);
		ImGui::Columns();

		if (m_Import)
		{
			auto& dst = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::AssetPanel>()->GetOpenDirectory();

			ImportImage(m_CurrentFilePath, dst, m_ImportDataBuffer.Read<ImageImportSettings>());

			m_Open = false;
			m_Import = false;
		}
	}
	void AssetImporterPanel::DrawMeshImportSettings()
	{
		MeshImportSettings info = m_ImportDataBuffer.Read<MeshImportSettings>();

		ImUI::ShiftY(4);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 200);

		ImUI::InputFloat("Scale factor", info.Scale);
		ImGui::Columns();

		if (m_Import)
		{
			auto& dst = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::AssetPanel>()->GetOpenDirectory();

			ImportMesh(m_CurrentFilePath, dst, m_ImportDataBuffer.Read<MeshImportSettings>());

			m_Open = false;
			m_Import = false;
		}
	}
	void AssetImporterPanel::DrawEnvironmentMapImportSettings()
	{
		ImUI::ShiftY(4);

		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(m_AssetHandle);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 200);

		auto& sourceImages = m_SelectableAssets["RadianceMap"];

		static uint64_t samples = map->GetSpec().Samples;
		static uint32_t resolution = Math::GetBaseLog(map->GetSpec().Resolution) - 6;
		uint32_t selectedImage = 0;
		static AssetHandle sourceImageHandle = map->GetSourceImageHandle();
		
		for (uint32_t i = 0; i < sourceImages.size(); i++)
		{
			if (sourceImages[i].Handle != sourceImageHandle) continue;
			selectedImage = i + 1;
			break;
		}


		ImGui::Text("Samples");
		ImGui::NextColumn();
		ImUI::Group("samples", [&]() {
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImUI::InputUInt(samples, 128, 1, 0);
		});

		ImGui::NextColumn();
		std::string options[] = { "64x64", "128x128", "256x256", "512x512", "1024x1024", "2048x2048", "4096x4096", "8192x8192" };
		ImUI::Combo("Resolution", "##res", options, 8, resolution);

		std::vector<std::string> displayNames;
		displayNames.reserve(m_SelectableAssets["RadianceMap"].size() + 1);
		displayNames.push_back("None");

		for (auto& metadata : m_SelectableAssets["RadianceMap"])
			displayNames.push_back(metadata.Key.c_str());

		if (ImUI::Combo("Source image", "##src", displayNames.data(), displayNames.size(), selectedImage))
		{
			if (selectedImage == 0) sourceImageHandle = INVALID_ASSET_HANDLE;
			else sourceImageHandle = m_SelectableAssets["RadianceMap"][selectedImage - 1].Handle;
		}
		ImGui::Columns();

		if (m_Import)
		{
			Application::Get().SubmitMainThread([&]() {
				Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(m_AssetHandle);
				map->Update(samples, (1 << 6 + resolution), sourceImageHandle);
				AssetManager::SaveAsset(map);
			});

			m_Open = false;
			m_Import = false;
		}
	}
	void AssetImporterPanel::InitializeData()
	{
		m_ImportDataBuffer.Release();
		m_SelectableAssets.clear();

		switch (m_AssetType)
		{
			case AssetType::Image:
			{
				ImageImportSettings settings = GetImportSettings<ImageImportSettings>(m_CurrentFilePath);
				settings.Handle = m_AssetHandle;

				m_ImportDataBuffer.Allocate(sizeof(ImageImportSettings));
				m_ImportDataBuffer.Write(&settings, sizeof(ImageImportSettings));
				break;
			}
			case AssetType::Mesh:
			{
				MeshImportSettings settings = GetImportSettings<MeshImportSettings>(m_CurrentFilePath);
				settings.Handle = m_AssetHandle;

				m_ImportDataBuffer.Allocate(sizeof(MeshImportSettings));
				m_ImportDataBuffer.Write(&settings, sizeof(MeshImportSettings));
				break;
			}
			case AssetType::EnvironmentMap:
			{
				m_SelectableAssets["RadianceMap"] = AssetManager::GetAllAssetMetadata(AssetType::Image);
				break;
			}
		}
	}

	void AssetImporterPanel::ImportImage(std::filesystem::path filePath, std::filesystem::path destination, ImageImportSettings settings)
	{
		//Build image import pipeline
		auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";

		Image2DCreateInfo info = {};
		info.GenerateMips = settings.GenerateMips;

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Import {}", File::GetName(filePath)), 2);
		Ref<JobGraph> packGraph = EditorAssetPackBuilder::CreatePackElement(filePath, info, settings);
		graph->CombineStages(packGraph);

		Ref<Job> saveJob = Ref<Job>::Create(EditorAssetPackBuilder::GenerateAndSaveAssetPack, packPath);
		graph->GetStage(1)->QueueJobs({ saveJob });
		Application::Get().GetJobSystem().QueueGraph<bool>(graph);
	}

	void AssetImporterPanel::ImportMesh(std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings)
	{
		auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";
		MeshCreateInfo info = {};

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Import {}", File::GetName(filePath)), 2);
		Ref<JobGraph> packGraph = EditorAssetPackBuilder::CreatePackElement(filePath, info, settings);

		Ref<GraphStage> firstStage = graph->GetStage(0);
		firstStage->SetWeight(0.95);
		graph->CombineStages(packGraph, 0);

		Ref<Job> saveJob = Ref<Job>::Create(EditorAssetPackBuilder::GenerateAndSaveAssetPack, packPath);
		Ref<GraphStage> saveStage = graph->GetStage(1);
		saveStage->SetWeight(0.05);
		saveStage->QueueJobs({ saveJob });

		Application::Get().GetJobSystem().QueueGraph<bool>(graph);
	}
}
