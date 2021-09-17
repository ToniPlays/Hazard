#pragma once

#include "../Mesh/VertexData.h"

#include "../Buffers/VertexArray.h"
#include "../Buffers/Buffer.h"
#include "../Pipeline/Shader.h"

#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "RenderData2D.h"


namespace Hazard::Rendering {

	class Renderer2D {
	public:
		Renderer2D(RenderEngineCreateInfo* info);
		~Renderer2D() = default;

		void Init(uint32_t size);

		void SubmitQuad(Quad quad);
		void BeginScene(glm::mat4 viewProjection);
		void BeginBatch();

		void Flush();
	private:
		float FindTexture(Texture* texture);
	private:
		Renderer2DData m_Data;
	};
}