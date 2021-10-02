#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Buffers/Buffers.h"
#include "Texture/Texture2D.h"

namespace Hazard::Rendering {

	struct RenderPassData {
		glm::mat4 ViewProjection;
		glm::mat4 Transform {1.0f};
	};

	class Renderer2D;

	class RenderEngine : public Module::Module
	{
	public:
		RenderEngine(RenderEngineCreateInfo* info, RenderAPI api);
		~RenderEngine();

		void Render() override;
		bool OnEvent(Event& e) override;
		void Close() override;

		bool OnResize(WindowResizeEvent& e);

		static std::filesystem::path GetShaderCompilePath() { return m_ShaderCompilePath; }
		static std::filesystem::path GetShaderSourcePath() { return m_ShaderSourcePath; }

	private:
		Renderer2D* m_Renderer2D;
		glm::mat4 m_ViewProjection;
		Ref<Texture2D> m_WhiteTexture;

		inline static std::filesystem::path m_ShaderCompilePath;
		inline static std::filesystem::path m_ShaderSourcePath;

	};
}