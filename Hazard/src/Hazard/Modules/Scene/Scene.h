#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Entity.h"
#include <vector>

namespace Hazard {

	class HAZARD_API Scene : public Entity {
	public:

		Scene(std::string name);
		~Scene() {};

		void Awake() {};
		void Start() {};
		void Update() {};
		void Render() {};

		void AddEntity(std::string name, Entity* entity = nullptr);
		void RemoveEntity(Entity* entity);
		std::vector<Entity*> GetEntities() { return entities; }

		std::string GetName() { return name; }
		void SetSceneName(std::string name) { this->name = name; }

	private:
		std::vector<Entity*> entities;
	};
}