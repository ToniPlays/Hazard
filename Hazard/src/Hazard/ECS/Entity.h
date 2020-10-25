#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Component.h"

namespace Hazard {

	class HAZARD_API Entity {
	public:
		Entity(std::string _name, std::initializer_list<Component*> _components);
		Entity(std::string _name);
		~Entity();

		void Render();
		void OnDestroy();
		template<typename T>
		void AddComponent() { components.push_back(new T()); }
		void RemoveComponent(Component* component);

		template<typename T>
		bool HasComponent() {
			return GetComponent<T>() != nullptr;
		}

		template<typename T>
		T* GetComponent() {
			for (Component* c : components) {
				if (dynamic_cast<T*>(c)) return (T*)c;
			}
			return nullptr;
		}
		std::vector<Entity*> GetChildEntities() { return childs; }
		std::vector<Component*> GetComponents() { return components; }

		std::string name = "";

	private:
		std::vector<Entity*> childs;
		std::vector<Component*> components;
	};
}