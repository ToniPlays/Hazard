#include "AssetImporterPanel.h"
#include "Core/EditorAssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include <spdlog/fmt/fmt.h>

#include "Core/GUIManager.h"
#include "GUI/ProjectPanel/AssetPanel.h"

#include "Hazard/ImGUI/UIElements/Dropdown.h"

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
		ImUI::ScopedColorStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);

		if (ImGui::Button(m_IsImportingNew ? "Import" : "Reimport", { 100, 32 }))
			m_Import = true;
	}
	void AssetImporterPanel::DrawImageImportSettings()
	{
		using namespace HazardRenderer;
		ImageImportSettings info = m_ImportDataBuffer.Read<ImageImportSettings>();

		ImUI::ShiftY(4);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));

		ImUI::Dropdown wrapMode("Texture wrap mode", 200);
		wrapMode.SetOptions({ "Repeat", "Repeat mirrored", "Clamp to border", "Clamp to edge" });
		wrapMode.SetSelected((uint32_t)info.Wrapping);
		wrapMode.Render();

		if (wrapMode.DidChange())
			info.Wrapping = (ImageWrap)wrapMode.GetSelected();

		ImUI::Dropdown minFilter("Texture min filter", 200);
		minFilter.SetOptions({ "Linear", "Nearest", "Linear mip", "Nearest mip" });
		minFilter.SetSelected((uint32_t)info.MinFilter);
		minFilter.Render();

		if (minFilter.DidChange())
			info.MinFilter = (FilterMode)minFilter.GetSelected();

		ImUI::Dropdown magFilter("Texture mag filter", 200);
		magFilter.SetOptions({ "Linear", "Nearest", "Linear mip", "Nearest mip" });
		magFilter.SetSelected((uint32_t)info.MagFilter);
		magFilter.Render();

		if (magFilter.DidChange())
			info.MagFilter = (FilterMode)magFilter.GetSelected();

		ImGui::Columns(2, 0, 200);
		ImUI::Checkbox("Generate mipmaps", info.GenerateMips);
		ImUI::Checkbox("Flip vertically", info.FlipOnLoad);
		ImGui::Columns();

		m_ImportDataBuffer.Write(&info, sizeof(ImageImportSettings));

		if (m_Import)
		{
			auto& dst = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::AssetPanel>()->GetOpenDirectory();

			ImportImage(m_CurrentFilePath, dst, info);

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

		//ImUI::InputFloatX("Scale factor", info.Scale);
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

		uint64_t samples = map->GetSpec().Samples;
		uint32_t resolution = Math::GetBaseLog(map->GetSpec().Resolution) - 6;
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
		ImGui::PushID("samples");
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImUI::InputUInt(samples, 128, 1, 0);
		ImGui::PopID();
		ImGui::NextColumn();
		ImGui::Columns();

		ImUI::Dropdown resolutions("Resolution", 200);
		resolutions.SetOptions({ "64x64", "128x128", "256x256", "512x512", "1024x1024", "2048x2048", "4096x4096", "8192x8192" });
		resolutions.SetSelected(resolution);
		resolutions.Render();

		if (resolutions.DidChange())
			resolution = resolutions.GetSelected();

		std::vector<std::string> displayNames;
		displayNames.reserve(m_SelectableAssets["RadianceMap"].size() + 1);
		displayNames.push_back("None");

		for (auto& metadata : m_SelectableAssets["RadianceMap"])
			displayNames.push_back(metadata.Key.c_str());

		ImUI::Dropdown radianceDropdown("Radiance", 200);
		radianceDropdown.SetOptions(displayNames);
		radianceDropdown.SetSelected(selectedImage);
		radianceDropdown.Render();

		if (radianceDropdown.DidChange() && radianceDropdown.GetSelected() != 0)
		{
			sourceImageHandle = m_SelectableAssets["RadianceMap"][radianceDropdown.GetSelected() - 1].Handle;
		}

		map->Update(samples, (1 << resolution + 6), sourceImageHandle);
		if (m_Import)
		{
			Application::Get().SubmitMainThread([&]() {
				Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(m_AssetHandle);
				map->Invalidate();

				AssetManager::SaveAsset(map);
				Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::AssetPanel>()->Refresh();
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
		JobPromise promise = AssetManager::DataFromSource(filePath);
		promise.Then([promise, destination, filePath]() {
			if (promise.Succeeded())
			{
				auto result = promise.GetResults<AssetPackElement>();
				auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";

				Application::Get().SubmitMainThread([packPath, filePath, result]() {

					AssetPack pack = {};
					pack.Handle = AssetHandle();
					pack.ElementCount = result.size();
					pack.Elements = result;

					for (auto& e : pack.Elements)
						e.AssetPackHandle = pack.Handle;

					CachedBuffer buffer = AssetPack::ToBuffer(pack);

					if (File::WriteBinaryFile(packPath, buffer.GetData(), buffer.GetSize()))
						AssetManager::ImportAssetPack(pack, packPath);
					pack.Free();

					Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>()->Refresh();
				});
			}
		});
	}

	void AssetImporterPanel::ImportMesh(std::filesystem::path filePath, std::filesystem::path destination, MeshImportSettings settings)
	{
		JobPromise promise = AssetManager::DataFromSource(filePath);
		promise.Then([promise, destination, filePath]() {
			if (promise.Succeeded())
			{
				auto result = promise.GetResults<AssetPackElement>();
				auto packPath = (destination / File::GetName(filePath)).lexically_normal().string() + ".hpack";

				Application::Get().SubmitMainThread([packPath, filePath, result]() {

					AssetPack pack = {};
					pack.Handle = AssetHandle();
					pack.ElementCount = result.size();
					pack.Elements = result;

					for (auto& e : pack.Elements)
						e.AssetPackHandle = pack.Handle;

					CachedBuffer buffer = AssetPack::ToBuffer(pack);

					if (File::WriteBinaryFile(packPath, buffer.GetData(), buffer.GetSize()))
						AssetManager::ImportAssetPack(pack, packPath);
					pack.Free();

					Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>()->Refresh();
				});
			}
		});
	}
}
