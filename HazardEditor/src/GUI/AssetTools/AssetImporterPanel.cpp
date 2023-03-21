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
	void AssetImporterPanel::OpenExisting(AssetHandle handle)
	{
		m_AssetHandle = handle;
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		m_AssetType = metadata.Type;
		m_Open = true;
		m_IsImportingNew = false;

		//Source file resolve by removing extension
		m_CurrentFilePath = metadata.Key.substr(0, metadata.Key.find_last_of('.'));

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
		ImageImportSettings& info = m_ImportDataBuffer.Read<ImageImportSettings>();

		ImUI::ShiftY(4);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 200);
		{
			ImGui::Text("Texture wrap mode");
			ImGui::NextColumn();

			const char* wrapMode[] = { "Repeat", "Repeat mirrored", "Clamp to border", "Clamp to edge" };
			uint32_t selected = (uint32_t)info.Wrapping;

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::Combo("##wrapMode", wrapMode, 4, selected))
			{
				info.Wrapping = (ImageWrap)selected;
			}
		}
		ImGui::NextColumn();
		const char* filter[] = { "Linear", "Nearest", "Linear mip", "Nearest mip" };

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
		MeshImportSettings& info = m_ImportDataBuffer.Read<MeshImportSettings>();

		ImUI::ShiftY(4);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 200);
		ImGui::Columns();

		if (m_Import)
		{
			auto& dst = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::AssetPanel>()->GetOpenDirectory();

			ImportMesh(m_CurrentFilePath, dst, m_ImportDataBuffer.Read<MeshImportSettings>());

			m_Open = false;
			m_Import = false;
		}
	}
	void AssetImporterPanel::InitializeData()
	{
		m_ImportDataBuffer.Release();
		switch (m_AssetType)
		{
		case AssetType::Image:
		{
			ImageImportSettings settings = {};
			settings.FlipOnLoad = false;
			settings.GenerateMips = true;
			settings.MinFilter = FilterMode::Linear;
			settings.MagFilter = FilterMode::Linear;
			settings.Wrapping = ImageWrap::ClampBorder;
			settings.Handle = m_AssetHandle;

			m_ImportDataBuffer.Allocate(sizeof(ImageImportSettings));
			m_ImportDataBuffer.Write(&settings, sizeof(ImageImportSettings));
			break;
		}
		case AssetType::Mesh:
		{
			MeshImportSettings settings = {};
			settings.Scale = 1.0f;
			settings.Handle = m_AssetHandle;

			m_ImportDataBuffer.Allocate(sizeof(MeshImportSettings));
			m_ImportDataBuffer.Write(&settings, sizeof(MeshImportSettings));
			break;
		}
		}
	}

	static void CreateImageMetadata(Ref<Job> job, Image2DCreateInfo info, AssetHandle handle, bool flip, const std::filesystem::path& path, const std::filesystem::path& destination)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "UID", handle);
		YamlUtils::Serialize(out, "Type", AssetType::Image);
		YamlUtils::Map(out, "Importer", [&]() {
			YamlUtils::Serialize(out, "WrapMode", (uint32_t)info.Filters.Wrapping);
		YamlUtils::Serialize(out, "MinFilter", (uint32_t)info.Filters.MinFilter);
		YamlUtils::Serialize(out, "MagFilter", (uint32_t)info.Filters.MagFilter);
		YamlUtils::Serialize(out, "FlipOnLoad", flip);
			});
		out << YAML::EndMap;
		File::WriteFile(destination.string() + ".meta", out.c_str());

		if (!File::Exists(destination) && path != destination)
			File::Copy(path, destination, CopyOptions::SkipExisting);
	}

	void AssetImporterPanel::ImportImage(std::filesystem::path filePath, std::filesystem::path destination, ImageImportSettings settings)
	{
		//Build image import pipeline
		auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";

		Image2DCreateInfo info = {};
		info.Filters = { settings.Wrapping, settings.MinFilter, settings.MagFilter };
		info.GenerateMips = settings.GenerateMips;

		Ref<Job> metadataJob = Ref<Job>::Create(CreateImageMetadata, info, settings.Handle, settings.FlipOnLoad, filePath, destination / File::GetName(filePath));
		metadataJob->SetJobTag("Metadata");

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Import {}", File::GetName(filePath)), 2);
		Ref<JobGraph> packGraph = EditorAssetPackBuilder::CreatePackElement(filePath, info, settings);

		graph->GetStage(0)->QueueJobs({ metadataJob });
		graph->CombineStages(packGraph);

		Ref<Job> saveJob = Ref<Job>::Create(EditorAssetPackBuilder::GenerateAndSaveAssetPack, packPath);
		graph->GetStage(1)->QueueJobs({ saveJob });
		Application::Get().GetJobSystem().QueueGraph<bool>(graph);
	}

	static void CreateMeshMetadata(Ref<Job> job, MeshImportSettings info, const std::filesystem::path& path, const std::filesystem::path& destination)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "UID", info.Handle);
		YamlUtils::Serialize(out, "Type", AssetType::Mesh);
		YamlUtils::Map(out, "Importer", [&]() {});
		out << YAML::EndMap;
		File::WriteFile(destination.string() + ".meta", out.c_str());

		if (!File::Exists(destination))
			File::Copy(path, destination, CopyOptions::SkipExisting);
	}

	void AssetImporterPanel::ImportMesh(std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings)
	{
		auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";
		MeshCreateInfo info = {};

		Ref<Job> metadataJob = Ref<Job>::Create(CreateMeshMetadata, settings, filePath, destination / File::GetName(filePath));
		metadataJob->SetJobTag("Metadata");

		Ref<JobGraph> graph = Ref<JobGraph>::Create(fmt::format("Import {}", File::GetName(filePath)), 3);
		Ref<JobGraph> packGraph = EditorAssetPackBuilder::CreatePackElement(filePath, info, settings);

		Ref<GraphStage> firstStage = graph->GetStage(0);
		firstStage->QueueJobs({ metadataJob });
		firstStage->SetWeight(0);

		Ref<GraphStage> secondStage = graph->GetStage(1);
		secondStage->SetWeight(0.95);
		graph->CombineStages(packGraph, 1);

		Ref<Job> saveJob = Ref<Job>::Create(EditorAssetPackBuilder::GenerateAndSaveAssetPack, packPath);
		Ref<GraphStage> saveStage = graph->GetStage(2);
		saveStage->SetWeight(0.05);
		saveStage->QueueJobs({ saveJob });

		Application::Get().GetJobSystem().QueueGraph<bool>(graph);
	}
}
