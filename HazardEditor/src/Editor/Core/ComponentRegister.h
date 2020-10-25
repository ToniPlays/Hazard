#pragma once
#include "Hazard.h"

using namespace Hazard;

struct ComponentInfo {
	std::string path;
	void(*fn)(Entity* entity);
	ComponentInfo() = default;
	ComponentInfo(std::string path, void(*fn)(Entity* entity)) {
		this->path = path;
		this->fn = fn;
	}
};


class ComponentRegister : public Module {
public:
	ComponentRegister();
	bool OnEnabled() override;
	std::vector<ComponentInfo> GetAll() { return components; }

private:
	std::vector<ComponentInfo> components;
};