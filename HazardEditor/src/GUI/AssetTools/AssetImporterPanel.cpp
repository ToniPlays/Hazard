
#include "AssetImporterPanel.h"

#include "GUI/AssetTools/AssetPanel.h"
#include "AssetImporters/ImageAssetImporter.h"
#include "AssetImporters/MeshAssetImporter.h"
#include "AssetImporters/EnvironmentAssetImporter.h"

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

		ImUI::Shift(4.0f, 4.0f);
		importer->RenderUI();

		ImVec2 size = ImGui::GetContentRegionMax();
		ImGui::SetCursorPos(ImVec2(size.x - 125, size.y - 48));
		ImGui::BeginChild("##bottombar", { size.x, 48 });
		if (ImGui::Button("Import", { 125, 48 }))
		{
			if (importer->Import())
				Close();
		}

		ImGui::EndChild();

	}
	void AssetImporterPanel::Open(AssetHandle handle)
	{
		using namespace Hazard;
		AssetMetadata& data = AssetManager::GetMetadata(handle);
		m_CurrentImportType = data.Type;

		if (!m_Importers.contains(m_CurrentImportType))
		{
			Close();
		}
		else
		{
			m_Importers[m_CurrentImportType]->Init(handle);
			m_Open = true;
		}
	}
	void AssetImporterPanel::Open(const std::filesystem::path& sourceFile)
	{
		using namespace Hazard;
		m_CurrentImportType = Utils::AssetTypeFromExtension(File::GetFileExtension(sourceFile));

		if (!m_Importers.contains(m_CurrentImportType))
		{
			Close();
		}
		else
		{
			m_Importers[m_CurrentImportType]->Init(sourceFile);
			m_Open = true;
		}
	}
}
