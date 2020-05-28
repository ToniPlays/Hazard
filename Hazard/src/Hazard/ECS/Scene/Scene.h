#pragma once

#include "Hazard/Core/Core.h"
#include "../GameObject.h"
#include <vector>

namespace Hazard {
	

	class HAZARD_API Scene {
	public:

		Scene();
		~Scene();

		void Awake();
		void Start();
		void Update();
		void Render();

	private:
		std::vector<

	};
}