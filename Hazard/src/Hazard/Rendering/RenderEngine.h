#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "RenderContext.h"
#include "Platform/System/Window.h"

#include "Textures/RenderTexture.h"
#include "Hazard/Entity/Scene.h"

#include "Buffers/VertexArray.h"
#include "Buffers/Buffer.h"
#include "Shader/Shader.h"



namespace Hazard::Rendering {
	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void Init() override;
		void Flush();

		void SceneRender(ECS::Scene& scene);
		void SetRenderTarget(RenderTexture* texture) { renderTarget = texture; };
		RenderTexture* GetRenderTarget() { return renderTarget; }

	private:
		RenderContext* context;
		RenderTexture* renderTarget = nullptr;

		//Temporary
		VertexArray* vertexArray;
		Shader* shader;

	};
}