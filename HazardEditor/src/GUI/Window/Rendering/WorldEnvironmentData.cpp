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
		ECS::WorldData& data = ECS::SceneCommand::GetCurrentWorld().GetWorldData();

		Layout::Treenode("Environment", Style::GetTreeNodeDefaultFlags(), [&]() {
			static bool open = false;

			Layout::Table(2, false);
			Layout::SetColumnWidth(300);
			Layout::Text("Clear color");
			Layout::TableNext();
			Input::ColorPicker("Clear color", data.clearColor, open);
			Layout::EndTable();
		});
		Layout::Treenode("Lighting", Style::GetTreeNodeDefaultFlags(), [&]() {

		});
	}
}
