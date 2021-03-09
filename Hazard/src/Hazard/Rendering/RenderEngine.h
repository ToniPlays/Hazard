#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "Platform/System/Window.h"

#include "Textures/RenderTexture.h"
#include "Hazard/Entity/Scene.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

#include "2D/Renderer2D.h"

namespace Hazard::Rendering {	

	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void Init() override;
		void Close() override;

		void SceneRender(ECS::Scene& scene);
		//TODO: remove Scene render
		void SceneRender(ECS::Scene& scene, ECS::Camera camera);
		void SetRenderTarget(RenderTexture* texture) { renderTarget = texture; };

		RenderTexture* GetRenderTarget() { return renderTarget; }

		template<typename T>
		void Submit(T element) {};
		template<>
		void Submit<Quad>(Quad element) { renderer2D->SubmitQuad(element); }

	private:
		RenderContext* context;

		RenderTexture* renderTarget = nullptr;
		Renderer2D* renderer2D;
	};
}