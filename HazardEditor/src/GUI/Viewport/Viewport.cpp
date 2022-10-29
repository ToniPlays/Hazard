
#include "Viewport.h"
#include "Hazard/Math/Time.h"
#include "Editor/EditorWorldManager.h"
#include "Hazard/Rendering/RenderEngine.h"

using namespace HazardRenderer;

namespace UI
{
	Viewport::Viewport() : Panel("Viewport")
	{
		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "ViewportCamera";
		frameBufferInfo.SwapChainTarget = false;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		frameBufferInfo.Attachments = { { ImageFormat::RGBA, ImageFormat::Depth } };
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Width = 1280;
		frameBufferInfo.Height = 720;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = frameBufferInfo.DebugName;
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);

		m_EditorGrid.Invalidate(m_RenderPass);
	}
	void Viewport::Update()
	{
		HZR_PROFILE_FUNCTION();
		WorldCameraData cameraData = {};
		cameraData.Projection = m_EditorCamera.GetProjection();
		cameraData.View = m_EditorCamera.GetView();
		cameraData.OutputFrameBuffer = m_FrameBuffer;
		cameraData.RenderPass = m_RenderPass;
		cameraData.Width = m_Width;
		cameraData.Height = m_Height;
		cameraData.ZNear = m_EditorCamera.GetNearClipping();
		cameraData.ZFar = m_EditorCamera.GetFarClipping();

		Editor::EditorWorldManager::GetWorldRender()->SubmitCamera(cameraData);
		Editor::EditorWorldManager::GetWorldRender()->SubmitExtra([&]() {
			m_EditorGrid.Render(m_EditorCamera);
			});
	}
	void Viewport::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImUI::ScopedStyleStack s(ImGuiStyleVar_FrameBorderSize, 0, ImGuiStyleVar_FrameRounding, FLT_MAX, ImGuiStyleVar_FramePadding, ImVec2(6, 5));

		bool is2D = m_EditorCamera.Is2DEnabled();
		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Width = size.x;
			m_Height = size.y;
			m_EditorCamera.SetViewport(m_Width, m_Height);
		}

		if (m_CurrentImage == 0 || true)
			ImUI::Image(m_FrameBuffer->GetImage(), size);

		ImUI::DropTarget<AssetHandle>(AssetType::World, [](AssetHandle assetHandle) {
			Application::Get().SubmitMainThread([handle = assetHandle]() {
				AssetMetadata& meta = AssetManager::GetMetadata(handle);
				Application::GetModule<WorldHandler>().LoadWorld(meta.Path);
				});
			});

		m_Gizmos.RenderGizmo(m_EditorCamera, m_SelectionContext, size);
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
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 25);

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.BackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::BeginChild("##gizmoTools", { 92, 25 });
		ImGui::SameLine(0, 6);
		if (ImUI::ColoredButton(ICON_FK_ARROWS, backgroundColor, m_Gizmos.GetType() != Gizmo::Translate ? offColor : style.Colors.AxisX, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Translate);
		}
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton(ICON_FK_REPEAT, backgroundColor, m_Gizmos.GetType() != Gizmo::Rotate ? offColor : style.Colors.AxisY, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Rotate);
		}
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton(ICON_FK_EXPAND, backgroundColor, m_Gizmos.GetType() != Gizmo::Scale ? offColor : style.Colors.Warning, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Scale);
		}

		ImGui::EndChild();
		ImRect rect = ImGui::GetCurrentWindow()->InnerRect;
		m_WindowFocused = ImGui::IsWindowFocused();

		if (m_DrawStats)
			DrawStatsWindow();

		if (m_Gizmos.IsUsing()) return;

		ImGui::IsWindowFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Viewport::OnSelectionContextChange));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
		m_Gizmos.OnEvent(e);

		if (m_Hovered)
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
		if (!m_Hovered) return false;

		switch (e.GetKeyCode()) {
		case Key::D1:
			m_Gizmos.SetType(Gizmo::Translate);
			return true;
		case Key::D2:
			m_Gizmos.SetType(Gizmo::Rotate);
			return true;
		case Key::D3:
			m_Gizmos.SetType(Gizmo::Scale);
			return true;
		case Key::D4:
			m_Gizmos.SetType(Gizmo::Bounds);
			return true;
		case Key::G:
			m_Gizmos.SetGlobal(!m_Gizmos.IsGlobal());
			return true;
		case Key::F: {
			FocusOnEntity(m_SelectionContext);
			return true;
		case Key::F1:
			m_EditorGrid.SetVisible(!m_EditorGrid.IsVisible());
		}
		}
		return false;
	}

	void Viewport::DrawStatsWindow()
	{
		HZR_PROFILE_FUNCTION();
		ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 5);

		ImGui::SetCursorPosX(165);
		ImGui::BeginChild("##gameStats", { 225, 160 }, false);

		ImGui::Dummy({ 0, 3 });
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 80);
		ImGui::Text("Frametime");
		ImGui::NextColumn();

		ImGui::Text("%.3f ms", Time::s_DeltaTime * 1000.0f);
		ImGui::NextColumn();
		ImGui::Text("FPS");
		ImGui::NextColumn();

		ImGui::Text("%.2f", 1.0f / Time::s_DeltaTime);
		ImGui::NextColumn();

		const char* attachments[] = { "World", "Positions", "Normals", "Color/Specular", "Depth" };

		ImUI::Combo("Shading", "##shading", attachments, 5, m_CurrentImage);


		ImGui::Columns();
		ImGui::EndChild();
	}
}