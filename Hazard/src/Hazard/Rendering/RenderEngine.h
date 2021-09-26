#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard::Rendering {

	class Renderer2D;

	class RenderEngine : public Module::Module
	{
	public:
		RenderEngine(RenderEngineCreateInfo* info, RenderAPI api);
		~RenderEngine() = default;

		void Update() override;

		static std::filesystem::path GetShaderCompilePath() { return m_ShaderCompilePath; }
		static std::filesystem::path GetShaderSourcePath() { return m_ShaderSourcePath; }

	private:
		Renderer2D* m_Renderer2D;
		inline static std::filesystem::path m_ShaderCompilePath;
		inline static std::filesystem::path m_ShaderSourcePath;
	};
}