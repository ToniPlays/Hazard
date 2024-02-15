#pragma once

#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ECS/Entity.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"

namespace UI
{
	class MeshAssetEditorPanel : public Hazard::ImUI::Panel
	{
	public:
		MeshAssetEditorPanel();
		~MeshAssetEditorPanel() = default;

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		void SetMeshHandle(AssetHandle meshHandle);
	private:
		void CreateWorldRenderer();

	private:
		uint32_t m_Width = 0, m_Height = 0;

		Ref<Hazard::WorldRenderer> m_Renderer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position = { -10, 5, 0 };

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
		Ref<HazardRenderer::Sampler> m_ImageSampler;
	};
}