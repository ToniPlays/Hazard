#pragma once

#include "Buffer/Buffer.h"
#include "Batch.h"
#include "Components/Components.h"
#include "Color.h"

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
		Anchor FinalBounds;
		Color Color;

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
		Batch<GUIVertex> Batch;
	};
}