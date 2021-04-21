#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "Platform/System/Window.h"

#include "Hazard/Entity/World.h"
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
		void SetRenderTarget(RenderTexture* texture) { m_RenderTarget = texture; };

		RenderTexture* GetRenderTarget() { return m_RenderTarget; }

		template<typename T>
		void Submit(T element) {};
		template<>
		void Submit<Quad>(Quad element) { m_Renderer2D->SubmitQuad(element); }
		template<>
		void Submit<RenderableMesh>(RenderableMesh mesh) {
			Shader& shader = mesh.mesh->GetMaterial().GetShader();
			shader.Bind();
			shader.SetUniformMat4("viewProjection", m_ViewProjection);
			shader.SetUniformMat4("model", mesh.transform);
			shader.SetUniformVec3("cameraPos", m_CameraPosition);
			mesh.mesh->Render();
		}

	private:
		glm::mat4 m_ViewProjection;
		glm::vec3 m_CameraPosition;

		RenderTexture* m_RenderTarget = nullptr;
		Renderer2D* m_Renderer2D = nullptr;
	};
}