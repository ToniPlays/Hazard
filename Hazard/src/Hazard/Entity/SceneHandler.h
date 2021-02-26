#pragma once

#include "Hazard/Module/Module.h"

namespace Hazard::ECS {
	class SceneHandler : public Module::Module {
	public:
		SceneHandler();
		~SceneHandler();

		void Init() override;
		void Update() override;
		void Render() override;

		void LoadScene(const char* file);

	private:
		
	};
}