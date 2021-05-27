#pragma once

#include "hzreditor.h"
#include "WorldEnvironmentData.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;

namespace WindowElement {

	WorldEnvironmentData::WorldEnvironmentData() : EditorWindow(ICON_FK_GLOBE " World data")
	{
		SetActive(false);
	}
	WorldEnvironmentData::~WorldEnvironmentData()
	{

	}
	void WorldEnvironmentData::OnWindowRender()
	{

		using namespace Rendering;
		ECS::World& world = ECS::WorldCommand::GetCurrentWorld();

		Layout::Treenode("Environment", Style::GetTreeNodeDefaultFlags(), [&]() {

			const char* bgText[] = { "Color", "Skybox", "Environment map" };
			const char* currentBgText;
			currentBgText = bgText[(int)world.GetWorldData().background];
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
						ECS::WorldBackground bg = (ECS::WorldBackground)i;

						world.SetBackground(bg, "");
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			Layout::EndTable();
			
			if (world.GetWorldData().background == ECS::WorldBackground::Colored) {
				Layout::Table(2, false, "##col");
				Layout::SetColumnWidth(200);
				Layout::Text("Background color");
				Layout::TableNext();
				Input::ColorPicker("Background color", world.GetWorldData().renderer->m_Color, open);
				Layout::EndTable();
			}
			else if(world.GetWorldData().background == ECS::WorldBackground::Sky)
			{
				Texture* texture = RenderUtils::Get<Texture2D>().Raw();
				Layout::Text("Front");
				Input::TextureSlot(texture, [&]() {});
				Layout::Text("Back");
				Input::TextureSlot(texture, [&]() {});
				Layout::Text("Left");
				Input::TextureSlot(texture, [&]() {});
				Layout::Text("Right");
				Input::TextureSlot(texture, [&]() {});
				Layout::Text("Top");
				Input::TextureSlot(texture, [&]() {});
				Layout::Text("Bottom");
				Input::TextureSlot(texture, [&]() {});
			}
			else 
			{
				SkyboxBackgroundRenderer* rd = (SkyboxBackgroundRenderer*)world.GetWorldData().renderer;
				Layout::Text("Environment map");
				bool clicked = Input::TextureSlot(rd->GetRaw().Raw(), [&]() {
					float gamma = rd->GetSkybox()->GetGamma();
					Input::Slider("Gamma", gamma, 0.0f, 10.0f);
					rd->GetSkybox()->SetGamma(gamma);

				});

				if (clicked) {
					std::string fileName = File::OpenFileDialog("");
					if (fileName != "") {

						CubemapCreateInfo createInfo;
						createInfo.width = 2048;
						createInfo.height = 2048;
						createInfo.datatype = TextureDataType::HDR;
						createInfo.sides = { { CubeSide::All, fileName } };

						rd->SetCubemap(RenderUtils::Create<CubemapTexture>(createInfo));
					}
				}
			}
			
		});
		Layout::Treenode("Lighting", Style::GetTreeNodeDefaultFlags(), [&]() {

		});
	}
}
