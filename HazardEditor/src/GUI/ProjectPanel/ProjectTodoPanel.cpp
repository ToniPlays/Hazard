
#include "ProjectTodoPanel.h"
#include "Hazard/ImGUI/UIElements/Table.h"
#include "Core/HazardScriptEngine.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

namespace UI
{
	using namespace Hazard;

	void ProjectTodoPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		using namespace HazardScript;
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImUI::TextFieldWithHint(m_SearchVal, "Search...");

		ImVec2 size = ImGui::GetContentRegionAvail();

		Hazard::ScriptEngine& engine = Application::GetModule<Hazard::ScriptEngine>();

		for (auto& assembly : engine.GetAssemblies())
		{
			ImGui::Text("Assembly: %s", File::GetNameNoExt(assembly->GetSourcePath()).c_str());

			ImUI::Table<Ref<TodoAttribute>> table("Assembly", size);
			table.SetColumns({ "Status", "Message" });
			table.RowHeight(24.0f);
			table.RowContent([this](uint32_t, const Ref<TodoAttribute>& item) {
				DrawElement("", "", item);
			});

			table.Render();
			ImUI::ShiftY(4.0f);
		}
	}
	void ProjectTodoPanel::DrawElement(const std::string& className, const std::string& name, const Ref<TodoAttribute> attrib)
	{
		constexpr float rowHeight = 24.0f;
		std::string title = className;
		if (!name.empty())
			title += "." + name;

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::Separator({ 4.0, rowHeight }, style.Colors.AxisX);
		ImGui::SameLine();
		ImGui::Text("%s", title.c_str());
		ImGui::TableNextColumn();
		ImUI::ShiftX(4.0f);
		ImGui::Text("%s", attrib->Detail.c_str());
	}
}
