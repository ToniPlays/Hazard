#pragma once

#include <glm/glm.hpp>
#include "Ref.h"
#include "DrawList.h"
#include "HazardRendererCore.h"

namespace HazardRenderer 
{
	class RenderCommand 
	{
		friend class WindowsWindow;
	public:
		static void BeginWorld(WorldSettings settings);
		static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color, Ref<Texture2D> texture);

		static void Flush();

	private:
		static void Init(Window* window);

	private:
		inline static GraphicsContext* s_Context = nullptr;

		inline static DrawList s_DrawList;
	};
}