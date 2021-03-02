#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "RenderContext.h"
#include "Platform/System/Window.h"

#include "Textures/RenderTexture.h"
#include "Hazard/Entity/Scene.h"

#include "2D/Renderer2D.h"

namespace Hazard::Rendering {	

	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void Init() override;
		void Flush();

		void SceneRender(ECS::Scene& scene) { 
			auto [found, cam] = scene.GetSceneCamera();
			if (!found) return;
			SceneRender(scene, cam.viewProjection); 
		};
		void SceneRender(ECS::Scene& scene, glm::mat4 projection);
		void SetRenderTarget(RenderTexture* texture) { renderTarget = texture; };

		RenderTexture* GetRenderTarget() { return renderTarget; }
		RenderStats& GetStats() { return stats; }

		template<typename T>
		void Submit(T element) {};
		template<>
		void Submit<Quad>(Quad element) { renderer2D->SubmitQuad(element); }

	private:
		RenderContext* context;
		RenderTexture* renderTarget = nullptr;

		Renderer2D* renderer2D;
		RenderStats stats;
	};
}