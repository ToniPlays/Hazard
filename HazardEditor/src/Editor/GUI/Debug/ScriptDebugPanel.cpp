
#include "ScriptDebugPanel.h"
#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
    using namespace Hazard;

    void ScriptDebugPanel::OnPanelRender()
    {
        
        struct TypeData
        {
            std::string Assembly;
            Coral::Type* Type;
        };
        
        HZR_PROFILE_FUNCTION();
        
        ScriptEngine& engine = Application::Get().GetModule<ScriptEngine>();

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImUI::TextFieldWithHint(m_SearchValue, "Search...");

        ImVec2 size = ImGui::GetContentRegionAvail();

        ImUI::Table<TypeData> table("ScriptDebugTable", size);
        table.SetColumns({ "Assembly", "Type" });
        table.RowHeight(24.0f);
        table.RowContent([&](uint32_t, TypeData& type) {
            ImUI::ShiftX(4.0f);
            ImGui::Text("%s", type.Assembly.c_str());
            ImGui::TableNextColumn();
            ImUI::ShiftX(4.0f);
            ImGui::Text("%s", std::string(type.Type->GetFullName()).c_str());
        });

        for (auto& assembly : engine.GetAssemblies())
        {
            for(auto& type : assembly->GetTypes())
                table.AddRow({ assembly->GetName(), type });
        }
        table.Render();
    }
}
