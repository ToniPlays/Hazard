#pragma once

#include "Hazard/ECS/Entity.h"

template<typename T>
bool AllEntitiesContain(std::vector<Hazard::Entity>& entities)
{
	for (auto& entity : entities)
		if (!entity.HasComponent<T>())
			return false;
	return true;
}
template<typename T>
void AddComponentToAll(std::vector<Hazard::Entity>& entities)
{
	for (auto& entity : entities)
		if (!entity.HasComponent<T>()) 
			entity.AddComponent<T>();
}
template<typename T>
void RemoveComponentFromAll(std::vector<Hazard::Entity>& entities)
{
	for (auto& entity : entities)
		if (entity.HasComponent<T>())
			entity.RemoveComponent<T>();
}
