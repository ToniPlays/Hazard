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
		void AddComponent() {
			T* component = new T();
			AddComponent(component);
		}

		void AddComponent(Component* component);
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

		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);

		std::string name = "";

	protected:
		std::vector<Entity*> childs;;
		std::vector<Component*> components;
	};
}