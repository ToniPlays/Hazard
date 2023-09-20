#pragma once

#include "Hazard/ECS/Entity.h"

template<typename T>
bool AllEntitiesContain(const std::vector<Hazard::Entity>& entities)
{
	for (auto entity : entities)
		if (!entity.HasComponent<T>())
			return false;
	return true;
}
template<typename T>
void AddComponentToAll(const std::vector<Hazard::Entity>& entities)
{
	for (auto entity : entities)
		if (!entity.HasComponent<T>()) 
			entity.AddComponent<T>();
}
template<typename T>
void RemoveComponentFromAll(const std::vector<Hazard::Entity>& entities)
{
	for (auto entity : entities)
		if (entity.HasComponent<T>())
			entity.RemoveComponent<T>();
}
