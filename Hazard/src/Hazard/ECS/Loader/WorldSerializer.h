#pragma once

#include "Hazard/Core/Core.h"
#include "../WorldHandler.h"
#include "../Entity.h"
#include "WorldSerializer.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

#include "Hazard/Scripting/ScriptSerializer.h"


namespace Hazard
{
	class WorldSerializer
	{
	public:

		WorldSerializer(Ref<World> world) : m_World(world) {}

		template<typename T>
		void TrySerialize(Entity& entity, YAML::Emitter& out)
		{
			if (entity.HasComponent<T>())
				SerializeComponent(entity, entity.GetComponent<T>(), out);
		}
		std::string Serialize();
		void SerializeEntity(Entity& entity, YAML::Emitter& out);
		template<typename T>
		void SerializeComponent(Entity& entity, T& component, YAML::Emitter& out) {};

	private:
		Ref<World> m_World;

	public:

		template<>
		void SerializeComponent(Entity& entity, TagComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "TagComponent", [&]() {
				YamlUtils::Serialize(out, "Tag", component.Tag);
			});
		}

		template<>
		void SerializeComponent(Entity& entity, TransformComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "TransformComponent", [&]() {
				YamlUtils::Serialize(out, "Translation", component.GetTranslation());
				YamlUtils::Serialize(out, "Rotation", component.GetRotation());
				YamlUtils::Serialize(out, "Scale", component.GetScale());
			});
		}
		template<>
		void SerializeComponent(Entity& entity, CameraComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "CameraComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Projection", component.GetProjectionType() == Projection::Orthographic ? "Orthographic" : "Perspective");
				YamlUtils::Serialize(out, "Fov", component.GetFov());
				YamlUtils::Serialize(out, "Size", component.GetSize());
				YamlUtils::Serialize(out, "Clipping", component.GetClipping());
			});
		}
		template<>
		void SerializeComponent(Entity& entity, ScriptComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "ScriptComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "ModuleName", component.ModuleName);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, SkyLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "SkyLightComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
				YamlUtils::Serialize(out, "SkyboxLod", component.LodLevel);
				YamlUtils::Serialize<AssetHandle>(out, "EnvironmentMap", component.EnvironmentMapHandle);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, DirectionalLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "DirectionalLightComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Color", component.LightColor);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, PointLightComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "PointLightComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Color", component.LightColor);
				YamlUtils::Serialize(out, "Intensity", component.Intensity);
				YamlUtils::Serialize(out, "Radius", component.Radius);
			});
		}
		template<>
		void SerializeComponent<MeshComponent>(Entity& entity, MeshComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "MeshComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Mesh", component.MeshHandle);
				YamlUtils::Serialize(out, "Material", component.MaterialHandle);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, SpriteRendererComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "SpriteRendererComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Color", component.Color);
				YamlUtils::Serialize(out, "Sprite", component.TextureHandle);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, Rigidbody2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "Rigidbody2DComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Type", Physics::BodyTypeToString(component.Type));
				YamlUtils::Serialize(out, "FixedRotation", component.FixedRotation);
				YamlUtils::Serialize(out, "UseGravity", component.UseGravity);
			});
		}
		template<>
		void SerializeComponent(Entity& entity, BoxCollider2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "BoxCollider2DComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
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
		void SerializeComponent(Entity& entity, CircleCollider2DComponent& component, YAML::Emitter& out)
		{
			YamlUtils::Map(out, "CircleCollider2DComponent", [&]() {
				YamlUtils::Serialize(out, "Active", component.Active);
				YamlUtils::Serialize(out, "Offset", component.Offset);
				YamlUtils::Serialize(out, "Radius", component.Radius);
				YamlUtils::Serialize(out, "Density", component.Density);
				YamlUtils::Serialize(out, "Friction", component.Friction);
				YamlUtils::Serialize(out, "Restitution", component.Restitution);
				YamlUtils::Serialize(out, "RestitutionThreshold", component.RestitutionThreshold);
				YamlUtils::Serialize(out, "IsSensor", component.IsSensor);
			});
		}
	};
}
