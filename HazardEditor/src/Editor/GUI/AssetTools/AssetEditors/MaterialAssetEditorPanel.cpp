#include "MaterialAssetEditorPanel.h"
#include <Hazard/Rendering/RenderEngine.h>
#include "Hazard/ECS/Entity.h"

namespace UI
{
	using namespace HazardRenderer;

	MaterialAssetEditorPanel::MaterialAssetEditorPanel() : Hazard::ImUI::Panel("Material editor")
	{
		FrameBufferCreateInfo frameBufferInfo = {
			.DebugName = "MeshAssetEditor",
			.Width = 1280,
			.Height = 720,
			.Samples = 1,
			.AttachmentCount = 3,
			.Attachments = { { ImageFormat::RGBA, { ImageFormat::RED32I, FramebufferBlendMode::None }, ImageFormat::Depth } },
			.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f },
			.SwapChainTarget = false,
		};

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {
			.DebugName = frameBufferInfo.DebugName,
			.pTargetFrameBuffer = m_FrameBuffer,
		};

		m_RenderPass = RenderPass::Create(&renderPassInfo);

		m_ImageSampler = Hazard::RenderEngine::GetResources().DefaultImageSampler;
		m_Projection = glm::perspective<float>(glm::radians(50.0), m_FrameBuffer->GetAspectRatio(), 0.03f, 400.0f);
		m_View = Math::ToTransformMatrix(m_Position, glm::angleAxis(glm::radians(-17.0f), glm::vec3 { 1, 0, 0 }));
		m_View = glm::inverse(m_View);

		CreateWorldRenderer();
	}

	void MaterialAssetEditorPanel::Update()
	{
		using namespace Hazard;

		WorldCameraData camera = {
			.Projection = m_Projection,
			.View = m_View,
			.Position = m_Position,
			.ZNear = 0.03,
			.ZFar = 400.0f,
			.Width = m_Width,
			.Height = m_Height,
			.RenderPass = m_RenderPass,
		};

		m_Renderer->SubmitCamera(camera);
		m_Renderer->Submit();
	}

	void MaterialAssetEditorPanel::OnPanelRender()
	{
		using namespace Hazard;

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImUI::ScopedStyleStack s(ImGuiStyleVar_FrameBorderSize, 0, ImGuiStyleVar_FrameRounding, FLT_MAX, ImGuiStyleVar_FramePadding, ImVec2(6, 5));

		ImVec2 corner = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Width = size.x;
			m_Height = size.y;
			m_Projection = glm::perspective<float>(glm::radians(60.0), (float)m_Width / (float)m_Height, 0.03f, 400.0f);
		}

		ImUI::Image(m_FrameBuffer->GetImage(), m_ImageSampler, size);

		RenderToolbar(corner);
	}

	bool MaterialAssetEditorPanel::OnEvent(Event& e)
	{
		return false;
	}

	void MaterialAssetEditorPanel::SetMaterialHandle(AssetHandle handle)
	{
		using namespace Hazard;
		Ref<World> world = m_Renderer->GetTargetWorld();
		auto view = world->GetEntitiesWith<MeshComponent>();

		for (auto& e : view)
		{
			Entity entity = world->GetEntity(e);
			entity.GetComponent<MeshComponent>().MaterialHandle = handle;
		}
	}

	void MaterialAssetEditorPanel::SetMeshHandle(AssetHandle handle)
	{
		using namespace Hazard;
		Ref<World> world = m_Renderer->GetTargetWorld();
		auto view = world->GetEntitiesWith<MeshComponent>();

		for (auto& e : view)
		{
			Entity entity = world->GetEntity(e);
			entity.GetComponent<MeshComponent>().MeshHandle = handle;
		}
	}

	void MaterialAssetEditorPanel::RenderToolbar(ImVec2 corner)
	{
		using namespace Hazard;
		float fontSize = ImGui::GetFontSize();
		float halfSize = fontSize * 0.5f;

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4& backgroundColor = style.ChildBackgroundColor;
		const ImVec4& offColor = style.Window.TextDisabled;

		ImGui::SetCursorPos({ corner.x + halfSize, corner.y + halfSize });

		ImUI::ScopedStyleVar rounding(ImGuiStyleVar_ChildRounding, fontSize + 16);

		ImGui::BeginChild("##materialtools", { (fontSize + 16) * 4, fontSize + 16 });
		ImGui::SameLine(0, 4);
		if (ImUI::ColoredButton((const char*)ICON_FK_STOP, backgroundColor, m_CurrentMode != 0 ? offColor : style.NavHighlight, { 0, fontSize + 16 }))
		{
			m_CurrentMode = 0;
			//SetMeshHandle(AssetManager::AssetHandleFromFile("res/Meshes/Cube.glb"));
		}

		ImGui::SameLine(0, 4);
		if (ImUI::ColoredButton((const char*)ICON_FK_CUBE, backgroundColor, m_CurrentMode != 1 ? offColor : style.NavHighlight, { 0, fontSize + 16 }))
		{
			m_CurrentMode = 1;
			SetMeshHandle(AssetManager::AssetHandleFromFile("res/Meshes/Cube.glb"));
		}

		ImGui::SameLine(0, 4);
		if (ImUI::ColoredButton((const char*)ICON_FK_DRIBBBLE, backgroundColor, m_CurrentMode != 2 ? offColor : style.NavHighlight, { 0, fontSize + 16 }))
		{
			m_CurrentMode = 2;
			SetMeshHandle(AssetManager::AssetHandleFromFile("res/Meshes/Sphere.glb"));
		}

		ImGui::SameLine(0, 4);
		if (ImUI::ColoredButton((const char*)ICON_FK_PLAY, backgroundColor, m_CurrentMode != 3 ? offColor : style.NavHighlight, { 0, fontSize + 16 }))
		{
			m_CurrentMode = 3;
			SetMeshHandle(AssetManager::AssetHandleFromFile("res/Meshes/Cone.glb"));
		}

		ImGui::EndChild();
	}

	void MaterialAssetEditorPanel::CreateWorldRenderer()
	{
		using namespace Hazard;

		Ref<World> world = Ref<World>::Create("Material editor world");
		Entity skylight = world->CreateEntity("Skylight");

		SkyLightComponent& comp = skylight.AddComponent<SkyLightComponent>();
		comp.EnvironmentMapHandle = AssetManager::AssetHandleFromFile("res/Textures/lythwood_terrace_4k.hdr");
		comp.Intensity = 4.0f;
		comp.LodLevel = 0.1f;

		Entity mesh = world->CreateEntity("Material Preview");
		auto& mc = mesh.AddComponent<MeshComponent>();
		mc.MeshHandle = AssetManager::AssetHandleFromFile("res/Meshes/Cube.glb");

		WorldRendererSpec spec = {
			.DebugName = "Mesh editor",
			.TargetWorld = world
		};

		m_Renderer = Ref<WorldRenderer>::Create(&spec);
	}
}