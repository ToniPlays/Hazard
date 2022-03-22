#pragma once

#include "Buffer.h"
#include "Batch.h"
#include "Anchor.h"
#include "Pivot.h"

#include <glm/glm.hpp>
#include <vector>

namespace HazardUI 
{
	struct GUIVertex 
	{
		glm::vec3 Position;
		glm::vec4 Color;
		float TextureIndex;
	};

	struct GuiDrawable 
	{
		Anchor Anchor;
		Pivot Pivot;

		GuiDrawable() {}
	};
	struct GuiDrawList 
	{
		std::vector<GuiDrawable> Drawables;
		
		void Clear() {
			Drawables.clear();
		}
		void Add(GuiDrawable drawable) { 
			Drawables.push_back(drawable); 
		}
	};
	struct GuiRawDrawable 
	{
		HazardUtility::Batch<GUIVertex> Batch;
	};
}