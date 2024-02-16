#pragma once

#include "Hazard/ImGUI/Panel.h"
#include "Hazard/Rendering/WorldRenderer.h"

namespace UI
{
	class MaterialAssetEditorPanel : public Hazard::ImUI::Panel
	{
	public:
		MaterialAssetEditorPanel();
		~MaterialAssetEditorPanel() = default;

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		void SetMaterialHandle(AssetHandle handle);
		void SetMeshHandle(AssetHandle handle);
		
	private:
		void RenderToolbar(ImVec2 corner);
		void CreateWorldRenderer();

	private:
		uint32_t m_Width = 0, m_Height = 0;

		Ref<Hazard::WorldRenderer> m_Renderer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position = { 0, 1.2, 3.5 };

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
		Ref<HazardRenderer::Sampler> m_ImageSampler;

		uint32_t m_CurrentMode = 1;
	};
}