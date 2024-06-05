#pragma once

#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ImGUI/UIElements/Dropdown.h"
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
		void RenderSidebar();
		void RenderToolbar(ImVec2 corner);
		void CreateWorldRenderer();
		void ListEnvironmentMaps();

	private:
		uint32_t m_Width = 0, m_Height = 0;

		Ref<Hazard::WorldRenderer> m_Renderer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position = { 0, 1.5, 4.5 };

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
		Ref<HazardRenderer::Sampler> m_ImageSampler;

		std::vector<Hazard::AssetMetadata> m_EnvironmentMaps;
		Hazard::ImUI::Dropdown m_EnvironmentDropdown;

		uint32_t m_CurrentMode = 1;
	};
}