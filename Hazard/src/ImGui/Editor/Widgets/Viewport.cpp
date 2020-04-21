#pragma once
#include <hzrpch.h>
#include "Viewport.h"

namespace Hazard {
	void Viewport::OnRender() const
	{
		if (!isLayerOpen) return;
		ImGui::Begin("Viewport");

		ImGui::End();
	}
}