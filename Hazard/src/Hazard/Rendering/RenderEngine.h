#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Platform/System/Window.h"

#include "Hazard/Entity/World.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Textures/EnvironmentMap.h"
#include "Sky/BackgroundRenderer.h"

#include "2D/Renderer2D.h"
#include "Camera.h"
#include "Mesh/Mesh.h"


namespace Hazard::Rendering {	

	struct CameraData {
		glm::mat4 viewProjection;
		glm::vec3 cameraPos;
		float gamma;
	};


	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void InitRenderer(RenderEngineCreateInfo* info);
		void Close() override;

		void BeginRendering(Camera camera, BackgroundRenderer& renderer);
		void EndRendering();
		void SetRenderTarget(RenderTexture* texture) { m_RenderTarget = texture; };

		RenderTexture* GetRenderTarget() { return m_RenderTarget; }

		template<typename T>
		void Submit(T element) {};
		template<>
		void Submit<Quad>(Quad element) { m_Renderer2D->SubmitQuad(element); }
		template<>
		void Submit<RenderableMesh>(RenderableMesh mesh) {

			Ref<Material> mat = mesh.mesh->GetMaterial();
			Shader* shader = mat->GetPipeline()->GetShader().Raw();

			shader->Bind();
			shader->SetUniformMat4("u_Model", mesh.transform);
			mesh.mesh->Render();
		}

	private:
		CameraData m_CameraData;

		Ref<UniformBuffer> m_CameraUnformBuffer;
		glm::mat4 m_Projection;
		glm::mat4 m_View;

		RenderTexture* m_RenderTarget = nullptr;
		Renderer2D* m_Renderer2D = nullptr;

		BackgroundRenderer* m_BackgroundRenderer;
	};
}