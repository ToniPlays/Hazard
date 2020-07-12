#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Vector/Transform.h"

namespace Hazard {

	class GameObject;

	class HAZARD_API Component {
	public:
		Component(GameObject* _gameObject, std::string _name) : gameObject(_gameObject), name(_name) {};
		virtual void OnRender() {};
	protected:
		GameObject* gameObject;
		std::string name;
	};

	class HAZARD_API GameObject {
	public:
		GameObject(std::string _name, std::initializer_list<Component*> _components, Transform transform = Transform());
		GameObject(std::string _name, Transform transform = Transform());
		~GameObject();

		Transform transform;
		std::string GetName() { return name; }
		void Render();
		void AddComponent(Component* component) { components.push_back(component); }

		template<typename T>
		T* GetComponent() {
			for (Component* c : components) {
				if (dynamic_cast<T*>(c)) return (T*)c;
			}
			return nullptr;
		}

	private:
		std::string name;
		std::vector<Component*> components;
	};
}