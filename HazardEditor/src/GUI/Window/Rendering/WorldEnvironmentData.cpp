#pragma once

#include "hzreditor.h"
#include "WorldEnvironmentData.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;
using namespace Appereance;

namespace WindowElement {

	WorldEnvironmentData::WorldEnvironmentData() : EditorWindow(ICON_FK_GLOBE " World data")
	{

	}
	WorldEnvironmentData::~WorldEnvironmentData()
	{

	}
	void WorldEnvironmentData::OnWindowRender()
	{

		using namespace Rendering;
		ECS::WorldData& data = ECS::SceneCommand::GetCurrentWorld().GetWorldData();

		Layout::Treenode("Environment", Style::GetTreeNodeDefaultFlags(), [&]() {

			const char* bgText[] = { "Color", "Skybox", "Environment map" };
			const char* currentBgText = bgText[(int)data.background];
			static bool open = false;

			Layout::Table(2, false, "##env");
			Layout::SetColumnWidth(200);
			Layout::Text("Type");
			Layout::TableNext();
			Layout::MaxWidth();

			if (ImGui::BeginCombo("##bgType", currentBgText)) {
				for (int i = 0; i < 3; i++) {

					bool isSelected = currentBgText == bgText[i];
					if (ImGui::Selectable(bgText[i], isSelected))
					{
						currentBgText = bgText[i];
						data.background = (ECS::WorldBackground)i;
						
						delete data.renderer;

						if (data.background == ECS::WorldBackground::Colored) {
							data.renderer = new ColorBackgroundRenderer();
						}
						else data.renderer = new SkyboxBackgroundRenderer();
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			Layout::EndTable();
			
			if (data.background == ECS::WorldBackground::Colored) {
				Layout::Table(2, false, "##col");
				Layout::SetColumnWidth(200);
				Layout::Text("Background color");
				Layout::TableNext();
				Input::ColorPicker("Background color", data.renderer->m_Color, open);
				Layout::EndTable();
			}
			else if(data.background == ECS::WorldBackground::Sky)
			{
				Ref<Texture2D> texture = RenderUtils::Create<Texture2D>("White");
				Layout::Text("Front");
				Input::TextureSlot(texture.Raw(), [&]() {});
				Layout::Text("Back");
				Input::TextureSlot(texture.Raw(), [&]() {});
				Layout::Text("Left");
				Input::TextureSlot(texture.Raw(), [&]() {});
				Layout::Text("Right");
				Input::TextureSlot(texture.Raw(), [&]() {});
				Layout::Text("Top");
				Input::TextureSlot(texture.Raw(), [&]() {});
				Layout::Text("Bottom");
				Input::TextureSlot(texture.Raw(), [&]() {});
			}
			else 
			{
				SkyboxBackgroundRenderer* rd = (SkyboxBackgroundRenderer*)data.renderer;
				Layout::Text("Environment map");
				bool clicked = Input::TextureSlot(rd->GetRaw().Raw(), [&]() {
					float gamma = rd->GetSkybox()->GetGamma();
					Input::Slider("Gamma", gamma, 0.0, 10.0f);
					rd->GetSkybox()->SetGamma(gamma);

				});

				if (clicked) {
					std::string fileName = File::OpenFileDialog("");
					if (fileName != "") {

						TextureSpecs specs;
						specs.width = 2048;
						specs.height = 2048;

						Ref<EnvinronmentMap>& newMap = RenderUtils::Create<EnvinronmentMap>(fileName.c_str(), specs);
						rd->SetCubemap(static_cast<Ref<CubemapTexture>>(newMap));
					}
				}
			}
			
		});
		Layout::Treenode("Lighting", Style::GetTreeNodeDefaultFlags(), [&]() {

		});
	}
}
