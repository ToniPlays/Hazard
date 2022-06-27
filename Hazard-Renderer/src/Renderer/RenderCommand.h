#pragma once

#include <glm/glm.hpp>
#include "Ref.h"
#include "DrawList.h"
#include "HazardRendererCore.h"

namespace HazardRenderer 
{
	struct RenderCommandResources {
		Ref<RawMesh> QuadMesh;
		Ref<VertexBuffer> QuadBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		Ref<VertexBuffer> InstanceBuffer;
	};

	class RenderCommand 
	{
		friend class WindowsWindow;
        friend class MacOSWindow;
	public:
		static void BeginFrame();
		static void BeginWorld(WorldSettings& settings);
		static void DrawQuad(Ref<Pipeline>& pipeline, const glm::mat4& transform, const glm::vec4& color);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform);
		//static void DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color, Ref<Texture2D> texture);

		static void Flush();
		static void RegisterPipelineDependency(Ref<Pipeline> pipeline);
		static const DrawList GetDrawList() { return s_DrawList; }
		static const RenderCommandResources& GetResources() { return s_Resources; };

	private:
		static void Init(Window* window);

	private:
		inline static GraphicsContext* s_Context = nullptr;
		inline static RenderCommandResources s_Resources;
		inline static DrawList s_DrawList;
		inline static Ref<RenderCommandBuffer> m_CommandBuffer;
	};
}
