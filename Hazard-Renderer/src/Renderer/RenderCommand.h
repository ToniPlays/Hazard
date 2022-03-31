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
		static void BeginFrame();
		static void BeginWorld(WorldSettings& settings);
		static void DrawQuad(Ref<Pipeline>& pipeline, const glm::mat4& transform, const glm::vec4& color);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color, Ref<Texture2D> texture);

		static void Flush();
		static void RegisterPipelineDependency(Ref<Pipeline> pipeline);

	private:
		static void Init(Window* window);

	private:
		inline static GraphicsContext* s_Context = nullptr;
		inline static Ref<RawMesh> m_QuadMesh;
		inline static Ref<VertexBuffer> m_InstanceBuffer;
		inline static DrawList s_DrawList;
		inline static Ref<RenderCommandBuffer> m_CommandBuffer;
	};
}