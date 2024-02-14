
#include "Viewport.h"
#include "Hazard/Math/Time.h"
#include "Editor/EditorWorldManager.h"
#include "Hazard/Rendering/HRenderer.h"

#include "MedianPoint.h"
#include "Core/EditorAssetManager.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/ImGUI/UIElements/Dropdown.h"
#include <Editor/EditorModeManager.h>

using namespace HazardRenderer;

namespace UI
{
	Viewport::Viewport() : Panel("Viewport", false)
	{
		FrameBufferCreateInfo frameBufferInfo = {
			.DebugName = "ViewportCamera",
			.Width = 1280,
			.Height = 720,
			.AttachmentCount = 3,
			.Attachments = { { ImageFormat::RGBA, { ImageFormat::RED32I, FramebufferBlendMode::None }, ImageFormat::Depth } },
			.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f },
			.SwapChainTarget = false,
		};

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = frameBufferInfo.DebugName;
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
		m_EditorGrid.Invalidate(m_RenderPass);

		m_ImageSampler = RenderEngine::GetResources().DefaultImageSampler;
	}
	void Viewport::Update()
	{
		HZR_PROFILE_FUNCTION();

		WorldCameraData cameraData = {
			.Projection = m_EditorCamera.GetProjection(),
			.View = m_EditorCamera.GetView(),
			.Position = m_EditorCamera.GetPosition(),
			.ZNear = m_EditorCamera.GetNearClipping(),
			.ZFar = m_EditorCamera.GetFarClipping(),
			.Width = m_Width,
			.Height = m_Height,
			.RenderPass = m_RenderPass,
		};

		auto renderer = Editor::EditorWorldManager::GetWorldRender();
		renderer->SubmitCamera(cameraData);

		Ref<World> world = renderer->GetTargetWorld();

		renderer->SubmitExtra([=]() mutable {
			HZR_PROFILE_FUNCTION("WorldRenderer::SubmitExtra()");
			m_EditorGrid.Render(m_EditorCamera);

			if (!world) return;

			if (m_ViewportSettings & ViewportSettingsFlags_CameraFrustum)
			{
				auto cameraView = world->GetEntitiesWith<CameraComponent>();
				if (cameraView.size() > 0)
				{
					auto icon = AssetManager::GetAsset<Texture2DAsset>(EditorAssetManager::GetIconHandle("Camera"));

					for (auto entity : cameraView)
					{
						Entity e = { entity, world.Raw() };
						auto& tc = e.GetComponent<TransformComponent>();
						auto& cc = e.GetComponent<CameraComponent>();

						if (cc.GetProjectionType() == Projection::Perspective)
							HRenderer::SubmitPerspectiveCameraFrustum(tc.GetTranslation(), tc.GetOrientation(), tc.GetTransformMat4(), cc.GetFov(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);
						else HRenderer::SubmitOrthoCameraFrustum(tc.GetTranslation(), tc.GetOrientation(), tc.GetTransformMat4(), cc.GetSize(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);

						if (m_ViewportSettings & ViewportSettingsFlags_LightIcons)
							HRenderer::SubmitBillboard(tc.GetTransformMat4(), m_EditorCamera.GetView(), Color::White, icon);
					}
				}
			}
			if (m_ViewportSettings & ViewportSettingsFlags_BoundingBox)
			{
				auto meshView = world->GetEntitiesWith<MeshComponent>();
				for (auto entity : meshView)
				{
					Entity e = { entity, world.Raw() };
					auto& tc = e.GetComponent<TransformComponent>();
					auto& mc = e.GetComponent<MeshComponent>();
					//if (mc.MeshHandle != INVALID_ASSET_HANDLE)
					//	HRenderer::SubmitBoundingBox(tc.GetTransformMat4(), AssetManager::GetAsset<Mesh>(mc.MeshHandle)->GetBoundingBox());
				}
			}
			if (m_ViewportSettings & ViewportSettingsFlags_LightIcons)
			{
				auto cameraView = world->GetEntitiesWith<DirectionalLightComponent>();
				if (cameraView.size() > 0)
				{
					auto icon = AssetManager::GetAsset<Texture2DAsset>(EditorAssetManager::GetIconHandle("DirectionalLight"));

					for (auto& entity : cameraView)
					{
						Entity e = { entity, world.Raw() };
						auto& tc = e.GetComponent<TransformComponent>();

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

		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Width = size.x;
			m_Height = size.y;
			//m_FrameBuffer->Resize(size.x, size.y);
			m_EditorCamera.SetViewport(m_Width, m_Height);
		}

		ImUI::Image(m_FrameBuffer->GetImage(), m_ImageSampler, size);
		if (ImGui::IsItemClicked())
		{
			m_DrawSettings = false;
			m_DrawStats = false;

			auto windowPos = ImGui::GetWindowPos();
			auto mousePos = Input::GetMousePos();
			glm::vec2 pos = { mousePos.x - windowPos.x, mousePos.y - windowPos.y - 36 };

			OnMouseClicked(pos);
		}

		ImUI::DropTarget<AssetHandle>(AssetType::World, [](AssetHandle assetHandle) {
			Application::Get().SubmitMainThread([handle = assetHandle]() mutable {
				Editor::EditorWorldManager::LoadWorld(handle);
			});
		});

		DrawLeftToolbar(corner);
		ImGui::SameLine();
		DrawPlaymodeButtons(size);
		ImGui::SameLine();
		DrawRightToolbar(size);

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

		switch (e.GetKeyCode())
		{
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
			case Key::F:
			{
				if (m_SelectionContext.size() > 0)
					FocusOnEntity(m_SelectionContext[0]);
				return true;
			case Key::F1:
				m_EditorGrid.SetVisible(!m_EditorGrid.IsVisible());
			}
		}
		return false;
	}
	void Viewport::OnMouseClicked(const glm::vec2& mousePos)
	{

	}

	void Viewport::DrawStatsWindow()
	{
		HZR_PROFILE_FUNCTION();
		ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 5);

		ImGui::SetCursorPosX(165);
		ImGui::BeginChild("##gameStats", { 225, 80 }, false);

		ImGui::Dummy({ 0, 3 });
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 80);
		ImGui::Text("Frametime");
		ImGui::NextColumn();
		ImGui::Text("%.3f ms", Time::s_DeltaTime * 1000.0);
		ImGui::NextColumn();

		ImGui::Text("FPS");
		ImGui::NextColumn();
		ImGui::Text("%.2f", 1.0 / Time::s_DeltaTime);
		ImGui::Columns();

		auto& renderEngine = Application::Get().GetModule<RenderEngine>();
		uint32_t flags = renderEngine.GetFlags();

		ImUI::Dropdown shading("Shading", 80);
		shading.SetOptions({ "Shaded", "Wireframe", "Shaded wireframe", "Overdraw" });
		shading.SetSelected(flags & RendererFlags_Overdraw ? 3 : 0);
		shading.Render();

		if (shading.DidChange())
		{
			switch (shading.GetSelected())
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
		ImGui::BeginChild("##settingsView", { 240, 160 }, false);

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

		ImGui::SetColumnWidth(0, 110);
		float cameraSpeed = m_EditorCamera.GetSpeedMultiplier();
		if (ImUI::SliderFloat("Camera speed", cameraSpeed, 1.0f, 0.01f, 5.0f))
			m_EditorCamera.SetSpeedMultiplier(cameraSpeed);

		ImGui::Columns();
		ImGui::EndChild();
	}
	void Viewport::DrawLeftToolbar(ImVec2 corner)
	{
		float size = ImGui::GetFontSize();
		float halfSize = size * 0.5f;

		ImGui::SetCursorPos({ corner.x + halfSize, corner.y + halfSize });

		if (ImGui::Button((const char*)ICON_FK_COG, { size + 12, size + 12 }))
			m_DrawSettings = !m_DrawSettings;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 2 });
		ImGui::SameLine(0, 8);
		std::string text = m_EditorCamera.Is2DEnabled() ? "2D Projection" : "3D Projection";

		if (ImGui::Button(text.c_str(), { 0, size + 12 }))
			m_EditorCamera.SetIs2D(!m_EditorCamera.Is2DEnabled());

		ImGui::SameLine(0, 8);
		if (ImGui::Button((const char*)ICON_FK_EYE " Show", { 0, size + 12 }))
			m_DrawStats = !m_DrawStats;

		ImGui::PopStyleVar();
	}
	void Viewport::DrawRightToolbar(ImVec2 size)
	{
		float fontSize = ImGui::GetFontSize();
		float halfSize = fontSize * 0.5f;

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.ChildBackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::SetCursorPosX(size.x - ((fontSize + 12) * 3 + halfSize));
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, fontSize + 12);

		ImGui::BeginChild("##gizmoTools", { (fontSize + 12) * 3, fontSize + 12});
		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton((const char*)ICON_FK_ARROWS, backgroundColor, m_Gizmos.GetType() != Gizmo::Translate ? offColor : style.Colors.AxisX, { 0, fontSize + 12 }))
			m_Gizmos.SetType(Gizmo::Translate);

		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton((const char*)ICON_FK_REPEAT, backgroundColor, m_Gizmos.GetType() != Gizmo::Rotate ? offColor : style.Colors.AxisY, { 0, fontSize + 12 }))
			m_Gizmos.SetType(Gizmo::Rotate);

		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton((const char*)ICON_FK_EXPAND, backgroundColor, m_Gizmos.GetType() != Gizmo::Scale ? offColor : style.Colors.Warning, { 0, fontSize + 12 }))
			m_Gizmos.SetType(Gizmo::Scale);

		ImGui::EndChild();
	}
	void Viewport::DrawPlaymodeButtons(ImVec2 size)
	{
		ImGui::SetCursorPosX((size.x - 84) * 0.5f);
		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, 8.0f);

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImVec4 backgroundColor = style.ChildBackgroundColor;
		backgroundColor.w = 0.8f;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColor);

		ImGui::BeginChild("##mode", { 90, 36 });
		ImGui::SameLine(0, 0);

		const Editor::EditorMode& mode = Editor::EditorModeManager::GetCurrentMode();

		if (mode == Editor::EditorMode::Edit)
			ImGui::BeginDisabled();
		if (ImUI::ColoredButton((const char*)ICON_FK_STOP, { 0, 0, 0, 0 }, style.Window.Text, { 30, 36 }))
		{
			Editor::EditorModeManager::EndPlayMode();
		}
		if (mode == Editor::EditorMode::Edit)
			ImGui::EndDisabled();

		ImGui::SameLine(0, 0);
		if (ImUI::ColoredButton((const char*)ICON_FK_PAUSE, { 0, 0, 0, 0 }, style.Window.Text, { 30, 36 }))
		{

		}
		ImGui::SameLine(0, 0);

		if (mode != Editor::EditorMode::Edit)
			ImGui::BeginDisabled();

		if (ImUI::ColoredButton((const char*)ICON_FK_PLAY, { 0, 0, 0, 0 }, style.Window.Text, { 30, 36 }))
		{
			Editor::EditorModeManager::BeginPlayMode();
		}
		if (mode != Editor::EditorMode::Edit)
			ImGui::EndDisabled();

		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}
