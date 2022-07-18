
#include "Viewport.h"
#include "Hazard/Math/Time.h"

namespace UI
{
	Viewport::Viewport() : Panel("Viewport")
	{
		WorldRendererSpec specs = {};
		specs.DebugName = "Viewport";
		specs.Width = m_Width;
		specs.Height = m_Height;
		specs.Camera = &m_EditorCamera;
		specs.Geometry = Geometry_All;
		m_Renderer = Ref<WorldRenderer>::Create(&specs);

		m_EditorGrid.Invalidate(m_Renderer->GetRenderPass());
	}
	void Viewport::Update()
	{
		m_Renderer->Render();
		m_EditorGrid.Render(m_EditorCamera);
	}
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

		UI::Image(m_Renderer->GetOutput()->GetImage(), size, { 0, 1 }, { 1, 0 });

		m_Gizmos.RenderGizmo(m_EditorCamera, m_SelectionContext, ImGui::GetWindowSize());

		ImGui::SetCursorPos({ corner.x + 10, corner.y + 5 });

		if (ImGui::Button(ICON_FK_COG, { 25, 25 }))
		{

		}

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
		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - 100);
		ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 25);

		const Style& style = StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.BackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::BeginChild("##gizmoTools", { 92, 25 });
		ImGui::SameLine(0, 6);
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
		ImRect rect = ImGui::GetCurrentWindow()->InnerRect;
		m_MouseOverWindow = ImGui::IsMouseHoveringRect(rect.Min, rect.Max, false);
		m_WindowFocused = ImGui::IsWindowFocused();

		if (m_DrawStats)
			DrawStatsWindow();

		if (m_Gizmos.IsUsing()) return;


		ImGui::IsWindowFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(HazardRenderer::Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Viewport::OnSelectionContextChange));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
		m_Gizmos.OnEvent(e);
		
		if (m_MouseOverWindow)
			m_EditorCamera.OnEvent(e);

		return false;
	}
	bool Viewport::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntity();
		return false;
	}

	bool Viewport::FocusOnEntity(Entity& entity)
	{
		if (!entity.IsValid()) return false;

		m_EditorCamera.SetFocalPoint(entity.GetTransform().Translation);
		return true;
	}

	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
		if (!m_WindowFocused) return false;

		switch (e.GetKeyCode()) {
		case Key::W:
			m_Gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::E:
			m_Gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::R:
			m_Gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::T:
			m_Gizmos.SetType(Gizmo::Bounds);
			return true;
		case Key::G:
			m_Gizmos.SetGlobal(!m_Gizmos.IsGlobal());
			return true;
		case Key::F: {
			FocusOnEntity(m_SelectionContext);
			return true;
		}
		}
		return false;
	}

	void Viewport::DrawStatsWindow()
	{
		Style& style = StyleManager::GetCurrent();
		ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);
		ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPosX(165);
		ImGui::BeginChild("##gameStats", { 225, 160 }, false);

		ApplicationData& data = Application::GetData();

		ImGui::Dummy({ 0, 3 });
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text("Frametime");
		ImGui::NextColumn();

		ImGui::Text("%.3f ms", Time::s_DeltaTime * 1000.0f);
		ImGui::NextColumn();
		ImGui::Text("FPS");
		ImGui::NextColumn();

		ImGui::Text("%.2f", 1.0f / Time::s_DeltaTime);
		ImGui::NextColumn();

		ImGui::Text("Memory");
		ImGui::NextColumn();
		ImGui::Text("%.2fmb", Application::GetData().MemoryUsage);

		ImGui::Columns();
		ImGui::EndChild();
	}
}