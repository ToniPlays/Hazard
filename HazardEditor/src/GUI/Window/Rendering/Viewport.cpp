#pragma once

#include "Viewport.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "Library/Style.h"
#include "Library/DragDropUtils.h"
#include "Core/EditorEvent.h"
#include "GUI/EditorView.h"
#include "Hazard/Rendering/WorldRenderer.h"
#include "../Inspect/Hierarchy.h"

#include "Platform/Vulkan/EditorPlatformVulkan.h"

using namespace Hazard;
using namespace WindowLayout;

namespace WindowElement {

	RenderEngine* engine = nullptr;

	Viewport::Viewport() : EditorWindow(ICON_FK_GLOBE " Viewport")
	{

	}
	void Viewport::Init()
	{
		SetActive(Application::HasModule<RenderEngine>());
		if (!IsActive()) return;

		WorldRendererSettings settings = {};
		settings.Camera = &m_EditorCamera;
		settings.ViewportSize = { 1920, 1080 };
		settings.ClearColor = Color::FromHex("#101010");
		settings.LineWidth = 5.0f;
		settings.Flags = WorldRenderFlags_::Enabled | WorldRenderFlags_::Geometry | WorldRenderFlags_::Quads | WorldRenderFlags_::Lines;
		m_WorldRenderer = WorldRenderer::Create(&settings);

		m_Grid.Invalidate(m_WorldRenderer->GetRenderPass());
	}
	void Viewport::OnUpdate()
	{
		m_Grid.Render(m_EditorCamera);
	}
	void Viewport::OnWindowRender()
	{
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();
		bool is2D = m_EditorCamera.Is2DEnabled();
		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		ScopedStyle framePadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		bool changed = false;

		if (size.x != m_Width || size.y != m_Height) {
			m_Width = size.x;
			m_Height = size.y;

			changed = true;
			m_EditorCamera.SetViewport(m_Width, m_Height);
			m_WorldRenderer->SetViewport(m_Width, m_Height);
		}

		Layout::Image(m_WorldRenderer->GetFinalPassImage(), size, ImVec2(0, 1), ImVec2(1, 0));

		DragDropUtils::DragTarget("World", [&](const ImGuiPayload* payload) {
			AssetHandle handle = *(AssetHandle*)payload->Data;
			Events::SelectionContextChange e({ });

			EditorView::GetInstance().OnEvent(e);
			AssetMetadata data = AssetManager::GetMetadata(handle);
			Application::GetModule<ECS::WorldHandler>().LoadWorld(data.Path.string());
			});

		m_Gizmos.RenderGizmo(m_EditorCamera, ImGui::GetWindowSize());

		ImGui::SetCursorPos({ corner.x + 10, corner.y + 5 });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, FLT_MAX);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 6, 5 });
		Style::SetButtonColors("#222222D0", "#181818D0", "#222222D0");

		if (Input::Button(ICON_FK_COG, { 25, 25 }))
		{

		}

		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 2 });
		ImGui::SameLine(0, 15);
		std::string text = m_EditorCamera.Is2DEnabled() ? "2D Projection" : "3D Projection";

		if (Input::Button(text.c_str(), { 0, 25 })) {
			m_EditorCamera.SetIs2D(!m_EditorCamera.Is2DEnabled());
		}

		ImGui::SameLine(0, 15);
		if (Input::Button(ICON_FK_EYE " Show", { 0, 25 }))
		{
			m_DrawStats = !m_DrawStats;
		}


		ImGui::PopStyleVar(2);
		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - 100);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Color::FromHex("#222222D0")));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 25);

		Color color("#22222200");
		Color offColor = Style::GetStyleColor(ColorType::Secondary);

		ImGui::BeginChild("##gizmoTools", { 92, 25 });
		ImGui::SameLine(0, 10);
		if (Input::ButtonColorChange(ICON_FK_ARROWS, offColor, Style::GetStyleColor(Debug), color, m_Gizmos.GetType() == Gizmo::Translate, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Translate);
		}
		ImGui::SameLine(0, 0);
		if (Input::ButtonColorChange(ICON_FK_REPEAT, offColor, Style::GetStyleColor(Critical), color, m_Gizmos.GetType() == Gizmo::Rotate, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Rotate);
		}
		ImGui::SameLine(0, 0);
		if (Input::ButtonColorChange(ICON_FK_EXPAND, offColor, Style::GetStyleColor(Warning), color, m_Gizmos.GetType() == Gizmo::Scale, { 0, 25 })) {
			m_Gizmos.SetType(Gizmo::Scale);
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(2);

		if (m_DrawStats)
			DrawStatsWindow();

		if (m_Gizmos.IsUsing()) return;

		IsFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(Hazard::Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		if (IsHovered())
			m_EditorCamera.OnEvent(e);
		m_Gizmos.OnEvent(e);

		if (!IsHovered()) return false;

		EventDispatcher dispacher(e);
		return dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
	}
	bool Viewport::FocusOnEntity(ECS::Entity entity)
	{
		if (!entity.IsValid()) return false;
		m_EditorCamera.SetFocalPoint(entity.GetTransform().Translation);
		return true;
	}
	bool Viewport::KeyPressed(KeyPressedEvent& e)
	{
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
			FocusOnEntity(Hierarchy::GetSelectedEntity());
			return true;
		}
		}
		return false;
	}
	void Viewport::DrawStatsWindow()
	{
		using namespace WindowLayout;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Background)));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5);
		ImGui::SetCursorPosX(165);
		ImGui::BeginChild("##gameStats", { 225, 160 }, false);

		const RenderStats& stats = RenderCommand::GetStats();
		ApplicationData& data = Application::GetData();

		Layout::NextLine(3);
		Layout::Table(2, true);
		Layout::SetColumnWidth(100);
		Layout::Text("Frametime");
		Layout::TableNext();

		std::stringstream ss;
		ss << Math::Round(Time::s_UnscaledDeltaTime * 1000.0f, 3) << "ms";
		Layout::Text(ss.str().c_str());
		ss.str("");
		Layout::TableNext();
		Layout::Text("FPS");
		Layout::TableNext();

		ss << Math::Round(1.0f / Time::s_UnscaledDeltaTime, 3);
		Layout::Text(ss.str().c_str());
		Layout::TableNext();

		Layout::Text("Memory");
		Layout::TableNext();
		ImGui::Text("%.2fmb", Application::GetData().MemoryUsage);

		Layout::TableNext();
		Layout::Text("Draw calls");
		Layout::TableNext();
		ImGui::Text("%u", stats.DrawCalls);

		Layout::TableNext();
		Layout::Text("Quads");
		Layout::TableNext();
		ImGui::Text("%u", stats.QuadCount);

		Layout::TableNext();
		Layout::Text("Vertices");
		Layout::TableNext();
		ImGui::Text("%u", stats.VertexCount);

		Layout::TableNext();
		Layout::Text("Indices");
		Layout::TableNext();
		ImGui::Text("%u", stats.IndexCount);

		Layout::EndTable();
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
}