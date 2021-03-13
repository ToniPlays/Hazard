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
			Layout::MenuItem("New C# script", [&]() {});
		});
	}
	void ContextMenus::HierarchyEntityMenu(Hazard::ECS::Scene& scene)
	{
		using namespace Hazard::ECS;
		using namespace Hazard::Rendering;
		Layout::ContextMenu([&scene]() {
			Layout::Menu("Entity", [&]() {
				Layout::MenuItem("Cube", [&]() {
					Entity entity = scene.CreateEntity("Cube");
					MeshComponent& component = entity.AddComponent<MeshComponent>();
					component.mesh = MeshFactory::LoadCube();
				});
				Layout::MenuItem("Icosphere", [&]() {
					Entity entity = scene.CreateEntity("Sphere");
					MeshComponent& component = entity.AddComponent<MeshComponent>();
					component.mesh = MeshFactory::LoadSphere();
				});
				Layout::MenuItem("Mesh", [&]() {
					Entity entity = scene.CreateEntity("Mesh");
					entity.AddComponent<MeshComponent>();
				});
			});
			Layout::Menu("2D", [&]() {
				Layout::MenuItem("Sprite", [&]() {
					Entity entity = scene.CreateEntity("Sprite");
					entity.AddComponent<SpriteRendererComponent>();
				});
			});
			Layout::Menu("Light", [&]() {
				Layout::MenuItem("Skylight", [&]() {
					Entity entity = scene.CreateEntity("Skylight");
					entity.AddComponent<SkyLightComponent>();
				});
				Layout::MenuItem("Directional light", [&]() {
					Entity entity = scene.CreateEntity("Directional Light");
					entity.AddComponent<DirectionalLightComponent>();
				});
				Layout::MenuItem("Point light", [&]() {
					Entity entity = scene.CreateEntity("Point Light");
					entity.AddComponent<PointLightComponent>();
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
			if (!entity.HasComponent<MeshComponent>())
				Layout::MenuItem("Mesh component", [&]() {
				entity.AddComponent<MeshComponent>();
					});
			});
	}
}