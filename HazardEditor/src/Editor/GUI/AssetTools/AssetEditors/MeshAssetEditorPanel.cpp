#include "MeshAssetEditorPanel.h"
#include <Hazard/Rendering/RenderEngine.h>
#include "Hazard/ECS/Entity.h"

namespace UI
{
	using namespace HazardRenderer;

	MeshAssetEditorPanel::MeshAssetEditorPanel() : Hazard::ImUI::Panel("Mesh editor")
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
		m_Projection = glm::perspective<float>(glm::radians(70.0), m_FrameBuffer->GetAspectRatio(), 0.03f, 400.0f);
		m_View = Math::ToTransformMatrix(m_Position, glm::angleAxis(-30.0f, glm::vec3 { 0, 1 ,0 }));

		m_Width = m_FrameBuffer->GetWidth();
		m_Height = m_FrameBuffer->GetHeight();

		CreateWorldRenderer();
	}

	void MeshAssetEditorPanel::Update()
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

	void MeshAssetEditorPanel::OnPanelRender()
	{
		using namespace Hazard;
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Projection = glm::perspective<float>(glm::radians(60.0), (float)m_Width / (float)m_Height, 0.03f, 400.0f);
			m_Width = size.x;
			m_Height = size.y;
		}

		ImUI::Image(m_FrameBuffer->GetImage(), m_ImageSampler, size);
	}

	bool MeshAssetEditorPanel::OnEvent(Event& e)
	{
		return false;
	}
	void MeshAssetEditorPanel::SetMeshHandle(AssetHandle meshHandle)
	{
		using namespace Hazard;

		Ref<World> world = m_Renderer->GetTargetWorld();
		auto view = world->GetEntitiesWith<MeshComponent>();

		for (auto& e : view)
		{
			Entity entity = world->GetEntity(e);
			entity.GetComponent<MeshComponent>().MeshHandle = meshHandle;
		}
	}
	void MeshAssetEditorPanel::CreateWorldRenderer()
	{
		using namespace Hazard;

		Ref<World> world = Ref<World>::Create("Mesh editor world");
		Entity skylight = world->CreateEntity("Skylight");

		SkyLightComponent& comp = skylight.AddComponent<SkyLightComponent>();
		comp.EnvironmentMapHandle = AssetManager::AssetHandleFromFile("res/Textures/lythwood_terrace_4k.hdr");
		comp.Intensity = 1.0f;
		comp.LodLevel = 0.5f;

		Entity mesh = world->CreateEntity("Mesh");
		auto& mc = mesh.AddComponent<MeshComponent>();
		mc.MeshHandle = INVALID_ASSET_HANDLE;

		WorldRendererSpec spec = {
			.DebugName = "Mesh editor",
			.TargetWorld = world
		};

		m_Renderer = Ref<WorldRenderer>::Create(&spec);
	}
}
