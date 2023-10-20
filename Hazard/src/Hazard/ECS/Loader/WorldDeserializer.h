#pragma once

#include "../Entity.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

#include "UID.h"

#include "Hazard/Assets/AssetEnums.h"
#include "Hazard/Scripting/ScriptSerializer.h"

namespace Hazard
{
	using DeserializerProgressHandler = std::function<void(Entity&, uint64_t, uint64_t)>;

	class WorldDeserializer
	{
	public:
		WorldDeserializer(AssetHandle handle) : m_Handle(handle) {}

		Ref<World> Deserialize();

		void SetProgressHandler(DeserializerProgressHandler handler) { m_Handler = handler; }

	private:
		template<typename T>
		static void TryDeserializeComponent(const char* key, Entity entity, YAML::Node node);
		template<typename T>
		static void Deserialize(Entity entity, YAML::Node node);

		template<>
		static void Deserialize<TagComponent>(Entity entity, YAML::Node comp)
		{
			YamlUtils::Deserialize(comp, "Tag", entity.GetComponent<TagComponent>().Tag, std::string("New entity"));
		};

		template<>
		static void Deserialize<TransformComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.GetComponent<TransformComponent>();

			glm::vec3 translation;
			glm::vec3 rotation;
			glm::vec3 scale;

			YamlUtils::Deserialize(comp, "Translation", translation, glm::vec3(0.0f));
			YamlUtils::Deserialize(comp, "Rotation", rotation, glm::vec3(0.0f));
			YamlUtils::Deserialize(comp, "Scale", scale, glm::vec3(1.0f));

			c.SetTranslation(translation);
			c.SetRotation(glm::radians(rotation));
			c.SetScale(scale);
		};

		template<>
		static void Deserialize<CameraComponent>(Entity entity, YAML::Node comp)
		{

			auto& c = entity.AddComponent<CameraComponent>();
			std::string projection;
			YamlUtils::Deserialize(comp, "Projection", projection, std::string("Perspective"));
			c.SetProjection(projection == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
			c.SetFov(comp["Fov"].as<float>());
			c.SetSize(comp["Size"].as<float>());

			glm::vec2 clipping;
			YamlUtils::Deserialize(comp, "Clipping", clipping, glm::vec2(0.03f, 1.0f));
			c.SetZNear(clipping.x);
			c.SetZFar(clipping.y);
		};

		template<>
		static void Deserialize<ScriptComponent>(Entity entity, YAML::Node comp)
		{
			std::string moduleName;
			bool active;

			YamlUtils::Deserialize(comp, "Active", active, true);
			YamlUtils::Deserialize(comp, "ModuleName", moduleName, std::string(""));

			auto& c = entity.AddComponentWithCallback<ScriptComponent>([&](ScriptComponent& c) {
				c.ModuleName = moduleName;
				c.Active = active;
			});

			if (!comp["Fields"] || !c.m_Handle)
				return;

			for (auto node : comp["Fields"])
			{
				std::string name = node.first.as<std::string>();
				ScriptSerializer::DeserializeFieldEditor(c.m_Handle, name, node.second);
			}
		};

		template<>
		static void Deserialize<SkyLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<SkyLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "SkyboxLod", c.LodLevel, 0.0f);
			YamlUtils::Deserialize<AssetHandle>(comp, "EnvironmentMap", c.EnvironmentMapHandle, INVALID_ASSET_HANDLE);
		}

		template<>
		static void Deserialize<DirectionalLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<DirectionalLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
		}

		template<>
		static void Deserialize<PointLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<PointLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "Radius", c.Radius, 1.0f);
		}

		template<>
		static void Deserialize<MeshComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<MeshComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);

			YamlUtils::Deserialize<AssetHandle>(comp, "Mesh", c.MeshHandle, INVALID_ASSET_HANDLE);
			YamlUtils::Deserialize<AssetHandle>(comp, "Material", c.MaterialHandle, INVALID_ASSET_HANDLE);
		};

		template<>
		static void Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<SpriteRendererComponent>();

			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			YamlUtils::Deserialize(comp, "Color", component.Color, Color::White);
			YamlUtils::Deserialize<AssetHandle>(comp, "Sprite", component.TextureHandle, INVALID_ASSET_HANDLE);
		};

		template<>
		static void Deserialize<Rigidbody2DComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<Rigidbody2DComponent>();
			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			std::string bodyType;
			YamlUtils::Deserialize(comp, "Type", bodyType, std::string("Static"));
			component.Type = Physics::StringToBodyType(bodyType);
			YamlUtils::Deserialize(comp, "FixedRotation", component.FixedRotation, false);
			YamlUtils::Deserialize(comp, "UseGravity", component.UseGravity, true);
		}

		template<>
		static void Deserialize<BoxCollider2DComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<BoxCollider2DComponent>();
			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			YamlUtils::Deserialize(comp, "Offset", component.Offset, glm::vec2(0.0f));
			YamlUtils::Deserialize(comp, "Size", component.Size, glm::vec2(1.0f));
			YamlUtils::Deserialize(comp, "Density", component.Density, 1.0f);
			YamlUtils::Deserialize(comp, "Friction", component.Friction, 1.0f);
			YamlUtils::Deserialize(comp, "Restitution", component.Restitution, 1.0f);
			YamlUtils::Deserialize(comp, "RestitutionThreshold", component.RestitutionThreshold, 0.1f);
			YamlUtils::Deserialize(comp, "IsSensor", component.IsSensor, false);
		}

		template<>
		static void Deserialize<CircleCollider2DComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<CircleCollider2DComponent>();
			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			YamlUtils::Deserialize(comp, "Offset", component.Offset, glm::vec2(0.0f));
			YamlUtils::Deserialize(comp, "Radius", component.Radius, 0.5f);
			YamlUtils::Deserialize(comp, "Density", component.Density, 1.0f);
			YamlUtils::Deserialize(comp, "Friction", component.Friction, 1.0f);
			YamlUtils::Deserialize(comp, "Restitution", component.Restitution, 1.0f);
			YamlUtils::Deserialize(comp, "RestitutionThreshold", component.RestitutionThreshold, 0.1f);
			YamlUtils::Deserialize(comp, "IsSensor", component.IsSensor, false);
		}

	private:

		AssetHandle m_Handle;
		DeserializerProgressHandler m_Handler;
	};
}
