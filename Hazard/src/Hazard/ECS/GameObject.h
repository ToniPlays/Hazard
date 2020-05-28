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
		GameObject(std::string _name, std::initializer_list<Component*> _components);
		GameObject(std::string _name);
		~GameObject();
		Transform transform;
		std::string GetName() { return name; }
		void Render();

	private:
		std::string name;
		std::vector<Component*> components;
	};
}