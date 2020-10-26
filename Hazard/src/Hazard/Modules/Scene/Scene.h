#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Entity.h"
#include <vector>

namespace Hazard {

	class HAZARD_API Scene : public Entity {
	public:

		Scene(std::string name = "Empty scene");
		~Scene() {};

		void Awake() {};
		void Start() {};
		void Update() {};
		void Render() {};

		void Flush();

		std::string GetName() { return name; }
		void SetSceneName(std::string name) { this->name = name; }
	};
}