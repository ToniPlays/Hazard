
#include "AssetImporterPanel.h"

#include "Editor/GUI/AssetTools/AssetPanel.h"
#include "AssetImporters/ImageAssetImporter.h"
#include "AssetImporters/MeshAssetImporter.h"
#include "AssetImporters/EnvironmentAssetImporter.h"

#include "Hazard/ImGUI/ScopedVar.h"

namespace UI
{
	AssetImporterPanel::AssetImporterPanel() : Hazard::ImUI::Modal("Asset importer", { 500, 600 })
	{
		RegisterImporter<ImageAssetImporter>(AssetType::Image);
		RegisterImporter<MeshAssetImporter>(AssetType::Mesh);
		RegisterImporter<EnvironmentAssetImporter>(AssetType::EnvironmentMap);
	}
	void AssetImporterPanel::OnPanelRender()
	{
		using namespace Hazard;

		if (m_CurrentImportType == AssetType::Undefined) return;

		auto& importer = m_Importers[m_CurrentImportType];
		DrawTopBar();
		importer->RenderUI();
		DrawBottomBar();
	}
	void AssetImporterPanel::Open(AssetHandle handle)
	{
		using namespace Hazard;
		AssetMetadata& data = AssetManager::GetMetadata(handle);
		m_CurrentImportType = data.Type;
		m_Source = data.SourceFile;

		if (!m_Importers.contains(m_CurrentImportType))
		{
			Close();
			return;
		}
		m_Importers[m_CurrentImportType]->Init(handle);
		m_Open = true;

		m_Title = fmt::format("{} Import options", Utils::AssetTypeToString(m_CurrentImportType));
	}
	void AssetImporterPanel::Open(const std::filesystem::path& sourceFile)
	{
		using namespace Hazard;
		m_CurrentImportType = Utils::AssetTypeFromExtension(File::GetFileExtension(sourceFile));
		m_Source = sourceFile;

		if (!m_Importers.contains(m_CurrentImportType))
		{
			Close();
			return;
		}

		m_Importers[m_CurrentImportType]->Init(sourceFile);
		m_Open = true;

		m_Title = fmt::format("New {} Import options", Utils::AssetTypeToString(m_CurrentImportType));
	}
	void AssetImporterPanel::DrawTopBar()
	{
		using namespace Hazard;
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImVec2 size = ImGui::GetWindowSize();

		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

		ImGui::BeginChild("##topbar", { size.x, 18 + 16 });
		ImUI::Shift(4.0f, 8.0f);
		{
			ImUI::ScopedFont font(ImGui::GetIO().Fonts->Fonts[1]);
			ImGui::Text("Current Asset: %s", File::GetName(m_Source).c_str());
		}
		ImGui::EndChild();
	}
	void AssetImporterPanel::DrawBottomBar()
	{
		using namespace Hazard;
		auto& importer = m_Importers[m_CurrentImportType];
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

		ImVec2 size = ImGui::GetContentRegionMax();
		ImGui::SetCursorPosY(size.y - 40);
		ImGui::BeginChild("##bottombar", { size.x, 40 });
		ImGui::SetCursorPosX(size.x - (96 * 3 + 15));
		ImUI::ShiftY(4.0f);
		{
			ImUI::ScopedFont font(ImGui::GetIO().Fonts->Fonts[1]);
			if (ImUI::ColoredButton("Import all", style.Colors.AxisZ, style.Window.Text, { 96, 32 }))
			{
				if (importer->Import())
					Close();
			}
		}

		ImGui::SameLine(0, 5);

		if (ImUI::ColoredButton("Import", style.ModalBackgroundColor, style.Window.Text, { 96, 32 }))
		{
			if (importer->Import())
				Close();
		}

		ImGui::SameLine(0, 5);
		if (ImUI::ColoredButton("Cancel", style.ModalBackgroundColor, style.Window.Text, { 96, 32 }))
			Close();

		ImGui::EndChild();
	}
}
