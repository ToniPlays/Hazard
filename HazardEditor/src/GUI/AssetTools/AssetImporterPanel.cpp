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
			//DrawMeshImportSettings();
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

			Ref<Job> job = Ref<Job>::Create(ImportImage, m_CurrentFilePath, dst, m_ImportDataBuffer.Read<ImageImportSettings>());
			job->SetJobTag("Texture");
			job->SetJobName(fmt::format("Import {}", File::GetName(m_CurrentFilePath)));

			Application::Get().GetJobSystem().QueueJob(job);
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

			Ref<Job> job = Ref<Job>::Create(ImportMesh, m_CurrentFilePath, dst, m_ImportDataBuffer.Read<MeshImportSettings>());
			job->SetJobTag("Mesh");
			job->SetJobName(fmt::format("Import {}", File::GetName(m_CurrentFilePath)));

			Application::Get().GetJobSystem().QueueJob(job);

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
	}
	void AssetImporterPanel::ImportImage(Ref<Job> job, std::filesystem::path filePath, std::filesystem::path destination, ImageImportSettings settings)
	{
		Image2DCreateInfo info = {};
		info.Filters = { settings.Wrapping, settings.MinFilter, settings.MagFilter };
		info.GenerateMips = settings.GenerateMips;

		{
			if (!File::Exists(destination / File::GetName(filePath)))
				File::Copy(filePath, destination / File::GetName(filePath), CopyOptions::SkipExisting);

			YAML::Emitter out;
			out << YAML::BeginMap;
			YamlUtils::Serialize(out, "UID", settings.Handle);
			YamlUtils::Serialize(out, "Type", AssetType::Image);
			YamlUtils::Map(out, "Importer", [&]() {
				YamlUtils::Serialize(out, "WrapMode", (uint32_t)info.Filters.Wrapping);
			YamlUtils::Serialize(out, "MinFilter", (uint32_t)info.Filters.MinFilter);
			YamlUtils::Serialize(out, "MagFilter", (uint32_t)info.Filters.MagFilter);
			YamlUtils::Serialize(out, "FlipOnLoad", settings.FlipOnLoad);
				});
			out << YAML::EndMap;
			File::WriteFile((destination / File::GetName(filePath)).string() + ".meta", out.c_str());
		}

		job->Progress(0.5f);

		AssetPackElement element = EditorAssetPackBuilder::CreatePackElement(filePath, info, settings.FlipOnLoad);
		element.Handle = settings.Handle;

		std::vector<AssetPackElement> elements = { element };
		AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
		CachedBuffer buffer = EditorAssetPackBuilder::AssetPackToBuffer(pack);

		//Save asset pack and generate meta file
		auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";
		File::WriteBinaryFile(packPath, buffer.GetData(), buffer.GetSize());

		//Finally import asset pack
		for (auto& e : pack.Elements)
		{
			e.AssetPack = packPath;
			AssetManager::ImportAsset(e, packPath);
		}
		pack.Free();
	}
	void AssetImporterPanel::ImportMesh(Ref<Job> job, std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings)
	{
		AssetHandle handle = AssetHandle();
		{
			File::Copy(filePath, destination / File::GetName(filePath), CopyOptions::SkipExisting);

			YAML::Emitter out;
			out << YAML::BeginMap;
			YamlUtils::Serialize(out, "UID", handle);
			YamlUtils::Serialize(out, "Type", AssetType::Mesh);
			YamlUtils::Map(out, "Importer", [&]() {});
			out << YAML::EndMap;
			File::WriteFile((destination / File::GetName(filePath)).string() + ".meta", out.c_str());
		}

		job->Progress(0.5f);

		MeshCreateInfo info = {};

		AssetPackElement element = EditorAssetPackBuilder::CreatePackElement(filePath, info);
		element.Handle = handle;

		std::vector<AssetPackElement> elements = { element };
		AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
		CachedBuffer buffer = EditorAssetPackBuilder::AssetPackToBuffer(pack);

		//Save asset pack and generate meta file
		auto packPath = (destination / File::GetNameNoExt(filePath)).string() + ".hpack";
		File::WriteBinaryFile(packPath, buffer.GetData(), buffer.GetSize());

		//Finally import asset pack
		for (auto& e : pack.Elements)
			AssetManager::ImportAsset(e, filePath.string());

		pack.Free();
	}
}
