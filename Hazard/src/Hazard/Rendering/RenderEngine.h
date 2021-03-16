#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "Platform/System/Window.h"

#include "Hazard/Entity/Scene.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Sky/Skybox.h"

#include "2D/Renderer2D.h"
#include "Camera.h"
#include "Mesh/Mesh.h"


namespace Hazard::Rendering {	

	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void Init() override;
		void Close() override;

		void BeginRendering(Camera camera);
		void EndRendering();
		void SetRenderTarget(RenderTexture* texture) { renderTarget = texture; };

		RenderTexture* GetRenderTarget() { return renderTarget; }

		template<typename T>
		void Submit(T element) {};
		template<>
		void Submit<Quad>(Quad element) { renderer2D->SubmitQuad(element); }
		template<>
		void Submit<RenderableMesh>(RenderableMesh mesh) {
			Shader& shader = mesh.mesh->GetMaterial().GetShader();
			shader.Bind();
			shader.SetUniformMat4("viewProjection", viewProjection);
			shader.SetUniformMat4("model", mesh.transform);
			shader.SetUniformVec3("cameraPos", cameraPosition);
			mesh.mesh->Render();
		}

	private:
		glm::mat4 viewProjection;
		glm::vec3 cameraPosition;

		RenderTexture* renderTarget = nullptr;
		Renderer2D* renderer2D = nullptr;
	};
}