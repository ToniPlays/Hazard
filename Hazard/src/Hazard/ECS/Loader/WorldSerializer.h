#pragma once

#include "../WorldHandler.h"
#include "../World.h"
#include "../Entity.h"
#include "WorldSerializer.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"


namespace Hazard
{
	class WorldSerializer {
	public:

		WorldSerializer(Ref<World> world) : m_World(world) {}

		template<typename T>
		void TrySerializeEditor(Entity& entity, YAML::Emitter& out)
		{
			if (entity.HasComponent<T>()) {
				SerializeComponentEditor(entity, entity.GetComponent<T>(), out);
			}
		}
		bool SerializeEditor(const std::string& file);
		void SerializeEntityEditor(Entity& entity, YAML::Emitter& out);
		template<typename T>
		void SerializeComponentEditor(Entity& entity, T& component, YAML::Emitter& out);


		bool SerializeRuntime(const std::string& file);

	private:
		Ref<World> m_World;

	public:

		template<>
		void SerializeComponentEditor(Entity& entity, TagComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "TagComponent", [&]() {
				YamlUtils::Serialize(out, "Tag", component.Tag);
				});
		}

		template<>
		void SerializeComponentEditor(Entity& entity, TransformComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "TransformComponent", [&]() {
				YamlUtils::Serialize(out, "Translation", component.Translation);
				YamlUtils::Serialize(out, "Rotation", component.Rotation);
				YamlUtils::Serialize(out, "Scale", component.Scale);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, CameraComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "CameraComponent", [&]() {
				YamlUtils::Serialize(out, "Projection", component.GetProjectionType() == Projection::Orthographic ? "Orthographic" : "Perspective");
				YamlUtils::Serialize(out, "Fov", component.GetFov());
				YamlUtils::Serialize(out, "Clipping", component.GetClipping());
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, ScriptComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "ScriptComponent", [&]() {
				YamlUtils::Serialize(out, "ModuleName", component.ModuleName);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, SkyLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "SkyLightComponent", [&]() {
				YamlUtils::Serialize(out, "Tint", component.LightColor);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, DirectionalLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "DirectionalLightComponent", [&]() {
				YamlUtils::Serialize(out, "Tint", component.LightColor);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, PointLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "PointLightComponent", [&]() {
				YamlUtils::Serialize(out, "Tint", component.LightColor);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
				YamlUtils::Serialize(out, "Radius", component.Radius);
				});
		}
		template<>
		void SerializeComponentEditor<MeshComponent>(Entity& entity, MeshComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "MeshComponent", [&]() {
				/*if (component.SourceAsset) {
					Ref<Hazard::Rendering::Mesh> mesh = AssetManager::GetRuntimeResource<Hazard::Rendering::Mesh>(component.SourceAsset->GetSourceHandle());
					if(mesh)
						YamlUtils::Serialize(out, "File", mesh->GetFile());
				}*/
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, SpriteRendererComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "SpriteRendererComponent", [&]() {
				YamlUtils::Serialize(out, "Tint", component.Tint);
				if (!component.Texture) return;

				AssetMetadata& meta = AssetManager::GetMetadata(component.Texture->GetHandle());
				YamlUtils::Serialize(out, "Texture", meta.Path.string());
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, Rigidbody2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "Rigidbody2DComponent", [&]() {
				YamlUtils::Serialize(out, "Type", Physics::BodyTypeToString(component.Type));
				YamlUtils::Serialize(out, "FixedRotation", component.FixedRotation);
				YamlUtils::Serialize(out, "UseGravity", component.UseGravity);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, BoxCollider2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "BoxCollider2DComponent", [&]() {
				YamlUtils::Serialize(out, "Offset", component.Offset);
				YamlUtils::Serialize(out, "Size", component.Size);
				YamlUtils::Serialize(out, "Density", component.Density);
				YamlUtils::Serialize(out, "Friction", component.Friction);
				YamlUtils::Serialize(out, "Restitution", component.Restitution);
				YamlUtils::Serialize(out, "RestitutionThreshold", component.RestitutionThreshold);
				YamlUtils::Serialize(out, "IsSensor", component.IsSensor);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, CircleCollider2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "CircleCollider2DComponent", [&]() {
				YamlUtils::Serialize(out, "Offset", component.Offset);
				YamlUtils::Serialize(out, "Radius", component.Radius);
				YamlUtils::Serialize(out, "Density", component.Density);
				YamlUtils::Serialize(out, "Friction", component.Friction);
				YamlUtils::Serialize(out, "Restitution", component.Restitution);
				YamlUtils::Serialize(out, "RestitutionThreshold", component.RestitutionThreshold);
				YamlUtils::Serialize(out, "IsSensor", component.IsSensor);
				});
		}
		template<>
		void SerializeComponentEditor(Entity& entity, BatchComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "BatchRendererComponent", [&]() {
				YamlUtils::Serialize(out, "Size", component.Size);
				YamlUtils::Serialize(out, "Tint", component.Tint);
				});
		}

	};
}
