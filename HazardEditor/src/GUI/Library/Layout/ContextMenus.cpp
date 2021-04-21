#pragma once

#include <hzreditor.h>
#include "ContextMenus.h"
#include "Layout.h"
#include "GUI/EditorView.h"

using namespace WindowLayout;

namespace WindowElement {
	void ContextMenus::FileContextMenu()
	{
		Layout::ContextMenu([&]() {
			Layout::MenuItem("New folder", [&]() {});
			Layout::MenuItem("New C# script", [&]() {});
			Layout::MenuItem("New scene", [&]() {});
		});
	}
	void ContextMenus::HierarchyEntityMenu(Hazard::ECS::World& world, void(*entityAdded)(Hazard::ECS::Entity))
	{
		using namespace Hazard::ECS;
		using namespace Hazard::Rendering;

		Layout::ContextMenu([&]() {
			Layout::Menu("Entity", [&]() {
				Layout::MenuItem("Cube", [&]() {
					Entity entity = world.CreateEntity("Cube");
					MeshComponent& component = entity.AddComponent<MeshComponent>();
					component.mesh = MeshFactory::LoadCube();
					entityAdded(entity);
				});
				Layout::MenuItem("Icosphere", [&]() {
					Entity entity = world.CreateEntity("Sphere");
					MeshComponent& component = entity.AddComponent<MeshComponent>();
					component.mesh = MeshFactory::LoadSphere();
					entityAdded(entity);
				});
				Layout::MenuItem("Mesh", [&]() {
					Entity entity = world.CreateEntity("Mesh");
					entity.AddComponent<MeshComponent>();
					entityAdded(entity);
				});
			});
			Layout::Menu("2D", [&]() {
				Layout::MenuItem("Sprite", [&]() {
					Entity entity = world.CreateEntity("Sprite");
					entity.AddComponent<SpriteRendererComponent>();
					entityAdded(entity);
				});
			});
			Layout::Menu("Light", [&]() {
				Layout::MenuItem("Skylight", [&]() {
					Entity entity = world.CreateEntity("Skylight");
					entity.AddComponent<SkyLightComponent>();
					entityAdded(entity);
				});
				Layout::MenuItem("Directional light", [&]() {
					Entity entity = world.CreateEntity("Directional Light");
					entity.AddComponent<DirectionalLightComponent>();
					entityAdded(entity);
				});
				Layout::MenuItem("Point light", [&]() {
					Entity entity = world.CreateEntity("Point Light");
					entity.AddComponent<PointLightComponent>();
					entityAdded(entity);
					});
				});
		});
	}
	void ContextMenus::PropertiesContextMenu(Hazard::ECS::Entity& entity)
	{
		using namespace Hazard::ECS;
		Layout::ContextMenu([&]() {
			if (!entity.HasComponent<CameraComponent>())
				Layout::MenuItem("Camera", [&]() {
				entity.AddComponent<CameraComponent>();
					});
			if (!entity.HasComponent<SpriteRendererComponent>())
				Layout::MenuItem("Sprite renderer", [&]() {
				entity.AddComponent<SpriteRendererComponent>();
					});
			if (!entity.HasComponent<SkyLightComponent>())
				Layout::MenuItem("Sky Light", [&]() {
				entity.AddComponent<SkyLightComponent>();
					});
			if (!entity.HasComponent<DirectionalLightComponent>())
				Layout::MenuItem("Directional light", [&]() {
				entity.AddComponent<DirectionalLightComponent>();
					});
			if (!entity.HasComponent<PointLightComponent>())
				Layout::MenuItem("Point light", [&]() {
				entity.AddComponent<PointLightComponent>();
					});
			if (!entity.HasComponent<BatchComponent>())
				Layout::MenuItem("Batch renderer", [&]() {
				entity.AddComponent<BatchComponent>();
					});
			if (!entity.HasComponent<ScriptComponent>())
				Layout::MenuItem("Script component", [&]() {
				entity.AddComponent<ScriptComponent>();
					});
			if (!entity.HasComponent<VisualScriptComponent>())
				Layout::MenuItem("Visual script component", [&]() {
				entity.AddComponent<VisualScriptComponent>();
					});
			if (!entity.HasComponent<MeshComponent>())
				Layout::MenuItem("Mesh component", [&]() {
				entity.AddComponent<MeshComponent>();
					});
			});
	}
}