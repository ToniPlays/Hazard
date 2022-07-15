
#include "Viewport.h"

namespace UI
{
	void Viewport::OnPanelRender()
	{
		ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ScopedStyleStack s(ImGuiStyleVar_FrameBorderSize, 0, ImGuiStyleVar_FrameRounding, FLT_MAX, ImGuiStyleVar_FramePadding, ImVec2(6, 5));

		bool is2D = m_EditorCamera.Is2DEnabled();
		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height) 
		{
			m_Width = size.x;
			m_Height = size.y;
			m_EditorCamera.SetViewport(m_Width, m_Height);
		}

		m_Gizmos.RenderGizmo(m_EditorCamera, m_SelectionContext, ImGui::GetWindowSize());

		ImGui::SetCursorPos({ corner.x + 10, corner.y + 5 });

		if (ImGui::Button(ICON_FK_COG, { 25, 25 }))
		{

		}

		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 2 });
		ImGui::SameLine(0, 15);
		std::string text = m_EditorCamera.Is2DEnabled() ? "2D Projection" : "3D Projection";

		if (ImGui::Button(text.c_str(), { 0, 25 })) {
			m_EditorCamera.SetIs2D(!m_EditorCamera.Is2DEnabled());
		}

		ImGui::SameLine(0, 15);
		if (ImGui::Button(ICON_FK_EYE " Show", { 0, 25 }))
		{
			m_DrawStats = !m_DrawStats;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - 100);
		ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 25);

		const Style& style = StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.BackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::BeginChild("##gizmoTools", { 92, 25 });
		ImGui::SameLine(0, 10);
		if (UI::ColoredButton(ICON_FK_ARROWS, backgroundColor, m_Gizmos.GetType() != Gizmo::Translate ? offColor : style.Colors.AxisX, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Translate);
		}
		ImGui::SameLine(0, 0);
		if (UI::ColoredButton(ICON_FK_REPEAT, backgroundColor, m_Gizmos.GetType() != Gizmo::Rotate ? offColor : style.Colors.AxisY, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Rotate);
		}
		ImGui::SameLine(0, 0);
		if (UI::ColoredButton(ICON_FK_EXPAND, backgroundColor, m_Gizmos.GetType() != Gizmo::Scale ? offColor : style.Colors.Warning, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Scale);
		}

		ImGui::EndChild();

		if (m_DrawStats)
			DrawStatsWindow();

		if (m_Gizmos.IsUsing()) return;

		bool focused = ImGui::IsWindowFocused();

		focused ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition({ 0, 0 });
	}
	bool Viewport::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Viewport::OnSelectionContextChange));
		return false;
	}
	bool Viewport::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntity();
		return false;
	}
	void Viewport::DrawStatsWindow()
	{
		Style& style = StyleManager::GetCurrent();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, style.BackgroundColor);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPosX(165);
		ImGui::BeginChild("##gameStats", { 225, 160 }, false);

		ApplicationData& data = Application::GetData();

		ImGui::Dummy({ 0, 3 });
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text("Frametime");
		ImGui::NextColumn();

		std::stringstream ss;
		ss << Math::Round(16.0f * 1000.0f, 3) << "ms";
		ImGui::Text(ss.str().c_str());
		ss.str("");
		ImGui::NextColumn();
		ImGui::Text("FPS");
		ImGui::NextColumn();

		ss << Math::Round(1.0f / 100.0f, 3);
		ImGui::Text(ss.str().c_str());
		ImGui::NextColumn();

		ImGui::Text("Memory");
		ImGui::NextColumn();
		ImGui::Text("%.2fmb", Application::GetData().MemoryUsage);

		ImGui::Columns();
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
}