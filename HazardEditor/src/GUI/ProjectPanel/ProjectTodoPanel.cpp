
#include "ProjectTodoPanel.h"
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
		const char* columns[] = { "Status", "Message" };

		for (auto& assembly : HazardScriptEngine::GetAssemblies())
		{
			ImGui::Text("Assembly: %s", File::GetNameNoExt(assembly->GetSourcePath()).c_str());

			ImUI::Table("TodoStatus", columns, 2, size, [&]() {
				for (auto& [name, metadata] : assembly->GetScripts())
				{
					auto& classAttrib = metadata->Get<TodoAttribute>();
					if (classAttrib)
						DrawElement(name, "", classAttrib);

					for (auto& [n, field] : metadata->GetFields())
					{
						auto& attrib = field->Get<TodoAttribute>();
						if (attrib) DrawElement(name, n, attrib);
					}
					for (auto& [n, method] : metadata->GetMethods())
					{
						auto& attrib = method->Get<TodoAttribute>();
						if (attrib) DrawElement(name, n, attrib);
					}
				}
				});
			ImUI::ShiftY(4.0f);
		}
	}
	void ProjectTodoPanel::DrawElement(const std::string& className, const std::string& name, const Ref<TodoAttribute> attrib)
	{
		constexpr float rowHeight = 24.0f;
		std::string title = className;
		if (!name.empty())
			title += "." + name;

		ImUI::TableRowClickable((const char*)name.c_str(), rowHeight);

		ImUI::Group((const char*)name.c_str(), [&]() {
			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
			ImUI::Separator({ 4.0, rowHeight }, style.Colors.AxisX);
			ImGui::SameLine();
			ImGui::Text("%s", title.c_str());
			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", attrib->Detail.c_str());
			});
	}
}
