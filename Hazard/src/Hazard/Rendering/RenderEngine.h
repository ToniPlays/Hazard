#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "RenderContext.h"
#include "Platform/System/Window.h"

namespace Hazard::Rendering {
	class RenderEngine : public Module::Module {
	public:
		RenderEngine();
		~RenderEngine();

		void PreInit() override;
		void Init() override;
		void Update() override;
		void Render() override;
		void Flush();

	private:
		RenderContext* context;
	};
}