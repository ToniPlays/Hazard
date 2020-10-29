#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Color.h"
#include "Hazard/ECS/Components/Mesh.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"

namespace Hazard {
	class HAZARD_API GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() const = 0;
		virtual void ClearFrame(Color clearColor = {34, 34, 34, 255}) const = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual std::string GetVersion() const = 0;
		virtual std::string GetError() const = 0;
		virtual void Draw(VertexArray* vertexArray, uint32_t indices, RenderType type = RenderType::Default) = 0;
		static GraphicsContext* Create(void* window, void* props);

	};
}