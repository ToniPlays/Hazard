#pragma once
#include "Core/Core.h"
#include "GameObject.h"

namespace Hazard {
	class HAZARD_API Scene {

	public:
		std::string name = "Unnamed";
	public:

		Scene(std::string _name);

		static Scene* GetCurrent() { return instance; }
		std::vector<GameObject*> GetAll() { return objects; }
		void AddGameObject(GameObject* object);

	private:
		static Scene* instance;
		std::vector<GameObject*> objects;
	};
}