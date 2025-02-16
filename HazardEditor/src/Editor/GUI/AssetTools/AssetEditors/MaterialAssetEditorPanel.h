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
		void OnOpen() override;

		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		void SetMeshHandle(AssetHandle handle);
		void SetMaterialHandle(AssetHandle handle);

	private:
		void RenderSidebar();
		void RenderToolbar(ImVec2 corner);
		void CreateWorldRenderer();
		void ListEnvironmentMaps();
		void SetEnvironmentMap(AssetHandle handle);

	private:
		uint32_t m_Width = 0, m_Height = 0;
		AssetHandle m_MaterialHandle = INVALID_ASSET_HANDLE;

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