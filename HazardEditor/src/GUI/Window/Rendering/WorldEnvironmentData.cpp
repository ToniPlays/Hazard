#pragma once

#include "WorldEnvironmentData.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/Input.h"
#include "Hazard/Entity/WorldCommand.h"

using namespace WindowLayout;

namespace WindowElement {

	WorldEnvironmentData::WorldEnvironmentData() : EditorWindow(ICON_FK_GLOBE " World data")
	{
		SetActive(false);
	}
	void WorldEnvironmentData::OnWindowRender()
	{

		using namespace Rendering;
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();
		
		Layout::Treenode("Lighting", Style::GetTreeNodeDefaultFlags(), [&]() {

		});
	}
}
