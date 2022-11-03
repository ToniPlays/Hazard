
#include "Viewport.h"
#include "Hazard/Math/Time.h"
#include "Editor/EditorWorldManager.h"
#include "Hazard/Rendering/HRenderer.h"

#include "MedianPoint.h"
#include "Core/EditorAssetManager.h"

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

		auto& renderer = Editor::EditorWorldManager::GetWorldRender();
		renderer->SubmitCamera(cameraData);

		Ref<World> world = renderer->GetTargetWorld();

		renderer->SubmitExtra([=]() mutable {
			HZR_PROFILE_FUNCTION("WorldRenderer::SubmitExtra()");
			m_EditorGrid.Render(m_EditorCamera);

			if (!world) return;

			if (m_ViewportSettings & ViewportSettingsFlags_CameraFrustum)
			{
				auto& cameraView = world->GetEntitiesWith<CameraComponent>();
				if (cameraView.size() > 0)
				{
					auto& icon = EditorAssetManager::GetIcon("Camera");

					for (auto entity : cameraView) {
						Entity e = { entity, world.Raw() };
						auto& tc = e.GetComponent<TransformComponent>();
						auto& cc = e.GetComponent<CameraComponent>();

						if (cc.GetProjectionType() == Projection::Perspective)
							HRenderer::SubmitPerspectiveCameraFrustum(tc.GetTranslation(), tc.GetOrientation(), tc.GetTransformMat4(), cc.GetFov(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);
						else HRenderer::SubmitOrthoCameraFrustum(tc.GetTranslation(), tc.GetOrientation(), tc.GetTransformMat4(), cc.GetSize(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);

						HRenderer::SubmitBillboard(tc.GetTransformMat4(), m_EditorCamera.GetView(), Color::White, icon);
					}
				}
			}
			if (m_ViewportSettings & ViewportSettingsFlags_BoundingBox)
			{
				auto& meshView = world->GetEntitiesWith<MeshComponent>();
				for (auto entity : meshView) {
					Entity e = { entity, world.Raw() };
					auto& tc = e.GetComponent<TransformComponent>();
					auto& mc = e.GetComponent<MeshComponent>();

					if (!mc.m_MeshHandle) continue;

					HRenderer::SubmitBoundingBox(tc.GetTransformMat4(), mc.m_MeshHandle->GetBoundingBox());
				}
			}
			if (m_ViewportSettings & ViewportSettingsFlags_LightIcons)
			{
				auto& cameraView = world->GetEntitiesWith<DirectionalLightComponent>();
				if (cameraView.size() > 0)
				{
					auto& icon = EditorAssetManager::GetIcon("DirectionalLight");

					for (auto& entity : cameraView)
					{
						Entity e = { entity, world.Raw() };
						auto& tc = e.GetComponent<TransformComponent>();
						auto& cc = e.GetComponent<DirectionalLightComponent>();

						HRenderer::SubmitBillboard(tc.GetTransformMat4(), m_EditorCamera.GetView(), Color::White, icon);
					}
				}
			}
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

		ImUI::Image(m_FrameBuffer->GetImage(), size);
		if (ImGui::IsItemClicked())
		{
			m_DrawSettings = false;
			m_DrawStats = false;
		}

		ImUI::DropTarget<AssetHandle>(AssetType::World, [](AssetHandle assetHandle) {
			Application::Get().SubmitMainThread([handle = assetHandle]() {
				AssetMetadata& meta = AssetManager::GetMetadata(handle);
				Editor::EditorWorldManager::LoadWorld(meta.Path);
				
				});
			});

		ImGui::SetCursorPos({ corner.x + 8, corner.y + 8 });

		if (ImGui::Button(ICON_FK_COG, { 28, 28 }))
			m_DrawSettings = !m_DrawSettings;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 2 });
		ImGui::SameLine(0, 8);
		std::string text = m_EditorCamera.Is2DEnabled() ? "2D Projection" : "3D Projection";

		if (ImGui::Button(text.c_str(), { 0, 28 })) {
			m_EditorCamera.SetIs2D(!m_EditorCamera.Is2DEnabled());
		}

		ImGui::SameLine(0, 8);
		if (ImGui::Button(ICON_FK_EYE " Show", { 0, 28 }))
			m_DrawStats = !m_DrawStats;

		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::SetCursorPosX(size.x - (84 + 8));
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 28);

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.ChildBackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::BeginChild("##gizmoTools", { 84, 28 });
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton(ICON_FK_ARROWS, backgroundColor, m_Gizmos.GetType() != Gizmo::Translate ? offColor : style.Colors.AxisX, { 0, 28 })) {
			m_Gizmos.SetType(Gizmo::Translate);
		}
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton(ICON_FK_REPEAT, backgroundColor, m_Gizmos.GetType() != Gizmo::Rotate ? offColor : style.Colors.AxisY, { 0, 28 })) {
			m_Gizmos.SetType(Gizmo::Rotate);
		}
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton(ICON_FK_EXPAND, backgroundColor, m_Gizmos.GetType() != Gizmo::Scale ? offColor : style.Colors.Warning, { 0, 28 })) {
			m_Gizmos.SetType(Gizmo::Scale);
		}

		ImGui::EndChild();
		ImRect rect = ImGui::GetCurrentWindow()->InnerRect;
		m_WindowFocused = ImGui::IsWindowFocused();

		if (m_DrawStats)
			DrawStatsWindow();
		if (m_DrawSettings)
			DrawSettingsWindow();

		if (m_SelectionContext.size() > 0)
		{
			MedianPoint point = {};
			point.Reset();

			for (auto& entity : m_SelectionContext)
			{
				auto& c = entity.GetComponent<TransformComponent>();
				point.Encapsulate(c.GetTranslation());
			}

			glm::mat4 target = glm::translate(glm::mat4(1.0f), point.GetMedianPoint());
			glm::mat4 deltaMatrix = m_Gizmos.RenderGizmo(m_EditorCamera, target, size);

			if (m_Gizmos.IsUsing())
			{
				for (auto& entity : m_SelectionContext)
				{
					auto& c = entity.GetComponent<TransformComponent>();
					c.SetTransform(deltaMatrix * c.GetTransformMat4());
				}
				return;
			}
		}

		ImGui::IsWindowFocused() ? m_EditorCamera.OnUpdate() : m_EditorCamera.SetMousePosition(Input::GetMousePos());
	}
	bool Viewport::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Viewport::OnSelectionContextChange));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Viewport::KeyPressed));
		if (m_Gizmos.OnEvent(e)) return true;

		if (m_Hovered)
			m_EditorCamera.OnEvent(e);

		return false;
	}
	bool Viewport::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntitites();
		return false;
	}

	bool Viewport::FocusOnEntity(Entity& entity)
	{
		if (!entity.IsValid()) return false;

		m_EditorCamera.SetFocalPoint(entity.GetTransform().GetTranslation());
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
			if (m_SelectionContext.size() > 0)
				FocusOnEntity(m_SelectionContext[0]);
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

		const char* shading[] = { "Shaded", "Wireframe", "Shaded wireframe", "Overdraw" };

		auto& renderEngine = Application::GetModule<RenderEngine>();
		uint32_t flags = renderEngine.GetFlags();
		uint32_t selected = 0;

		if (flags & RendererFlags_Overdraw)
			selected = 3;

		if (ImUI::Combo("Shading", "##shading", shading, 4, selected))
		{
			switch (selected)
			{
			case 0: flags = 0; break;
			case 3: flags |= RendererFlags_Overdraw; break;
			default: flags = 0; break;
			}
			renderEngine.SetFlags(flags);
		}

		ImGui::Columns();
		ImGui::EndChild();
	}
	void Viewport::DrawSettingsWindow()
	{
		HZR_PROFILE_FUNCTION();
		ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);
		ImUI::ScopedStyleStack rounding(ImGuiStyleVar_ChildRounding, 5, ImGuiStyleVar_FrameRounding, 4);

		ImGui::SetCursorPosX(12);
		ImGui::BeginChild("##settingsView", { 200, 160 }, false);

		ImGui::Dummy({ 0, 3 });

		bool boundingBox = m_ViewportSettings & ViewportSettingsFlags_BoundingBox;
		bool cameraFrustum = m_ViewportSettings & ViewportSettingsFlags_CameraFrustum;
		bool lightIcons = m_ViewportSettings & ViewportSettingsFlags_LightIcons;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 160);
		if (ImUI::Checkbox("Bounding boxes", boundingBox))
			m_ViewportSettings ^= ViewportSettingsFlags_BoundingBox;

		if (ImUI::Checkbox("Camera frustum", cameraFrustum))
			m_ViewportSettings ^= ViewportSettingsFlags_CameraFrustum;
		if (ImUI::Checkbox("Light icons", lightIcons))
			m_ViewportSettings ^= ViewportSettingsFlags_LightIcons;

		ImGui::Columns();
		ImGui::EndChild();
	}
}