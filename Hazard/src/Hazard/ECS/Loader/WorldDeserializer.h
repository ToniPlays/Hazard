#pragma once

#include "../Entity.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"
#include "Hazard/Assets/AssetEnums.h"
#include "Hazard/Scripting/ScriptSerializer.h"
#include "Hazard/Assets/Asset.h"

#include "UID.h"
#include "Callback.h"

namespace Hazard
{
	using DeserializerCallback = Callback<void(uint64_t, uint64_t)>;

	class WorldDeserializer
	{
	public:
		WorldDeserializer(const std::string& debugName) : m_DebugName(debugName) {};
		WorldDeserializer(const std::string& debugName, const std::string& source) : m_DebugName(debugName), m_Source(source) {}

		const std::string& GetDebugName() const { return m_DebugName; }
		void SetSource(const std::string& source) { m_Source = source; }

		Ref<World> Deserialize();
		std::unordered_map<AssetMetadata, uint32_t> GetReferencedAssets();

		void AddProgressHandler(std::function<void(uint64_t, uint64_t)> callback)
		{
			m_Handler.Add(callback);
		}

	private:
		template<typename T>
		static void TryDeserializeComponent(const char* key, Entity entity, const YAML::Node& comp);
		template<typename T>
		static void Deserialize(Entity entity, const YAML::Node& comp);

		template<typename T>
		static void TryGetReferencedAssets(const char* key, std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& node);

		template<typename T>
		static void GetReferencedAssets(std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& comp) {}

		template<>
		static void GetReferencedAssets<SkyLightComponent>(std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& comp)
		{
			AssetHandle handle = 0;
			if (YamlUtils::Deserialize(comp, "EnvironmentMap", handle, (AssetHandle)INVALID_ASSET_HANDLE))
				handles[handle] += 1;
		}
		template<>
		static void GetReferencedAssets<MeshComponent>(std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& comp)
		{
			AssetHandle handle = 0;
			if (YamlUtils::Deserialize(comp, "Mesh", handle, (AssetHandle)INVALID_ASSET_HANDLE))
				handles[handle] += 1;
			if (YamlUtils::Deserialize(comp, "Material", handle, (AssetHandle)INVALID_ASSET_HANDLE))
				handles[handle] += 1;
		}
		template<>
		static void GetReferencedAssets<SpriteRendererComponent>(std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& comp)
		{
			AssetHandle handle = 0;
			if (YamlUtils::Deserialize(comp, "Sprite", handle, (AssetHandle)INVALID_ASSET_HANDLE))
				handles[handle] += 1;
		}


		template<>
		static void Deserialize<TagComponent>(Entity entity, const YAML::Node& comp)
		{
			YamlUtils::Deserialize(comp, "Tag", entity.GetComponent<TagComponent>().Tag, std::string("New entity"));
		}

		template<>
		static void Deserialize<RelationshipComponent>(Entity entity, const YAML::Node& comp)
		{
			UID handle = 0;
			YamlUtils::Deserialize(comp, "Parent", handle, (UID)0);
			entity.SetParent(entity.GetWorld().TryGetEntityFromUID(handle));
		}

		template<>
		static void Deserialize<TransformComponent>(Entity entity, const YAML::Node& comp)
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
		static void Deserialize<CameraComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& c = entity.AddComponent<CameraComponent>();
			std::string projection;
			glm::vec2 clipping;

			YamlUtils::Deserialize(comp, "Projection", projection, std::string("Perspective"));
			YamlUtils::Deserialize(comp, "Clipping", clipping, glm::vec2(0.03f, 1.0f));

			c.SetProjection(projection == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
			c.SetZNear(clipping.x);
			c.SetZFar(clipping.y);
			c.SetFov(comp["Fov"].as<float>());
			c.SetSize(comp["Size"].as<float>());
		};

		template<>
		static void Deserialize<ScriptComponent>(Entity entity, const YAML::Node& comp)
		{
			std::string moduleName;
			bool active;

			YamlUtils::Deserialize(comp, "Active", active, true);
			YamlUtils::Deserialize(comp, "ModuleName", moduleName, std::string(""));
		};

		template<>
		static void Deserialize<SkyLightComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& c = entity.AddComponent<SkyLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "SkyboxLod", c.LodLevel, 0.0f);
			YamlUtils::Deserialize<AssetHandle>(comp, "EnvironmentMap", c.EnvironmentMapHandle, INVALID_ASSET_HANDLE);
		}

		template<>
		static void Deserialize<DirectionalLightComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& c = entity.AddComponent<DirectionalLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
		}

		template<>
		static void Deserialize<PointLightComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& c = entity.AddComponent<PointLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "Radius", c.Radius, 1.0f);
		}

		template<>
		static void Deserialize<MeshComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& c = entity.AddComponent<MeshComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize<AssetHandle>(comp, "Mesh", c.MeshHandle, INVALID_ASSET_HANDLE);
			YamlUtils::Deserialize<uint64_t>(comp, "Submesh", c.SubmeshHandle, 0);
			YamlUtils::Deserialize<AssetHandle>(comp, "Material", c.MaterialHandle, INVALID_ASSET_HANDLE);
		};

		template<>
		static void Deserialize<SpriteRendererComponent>(Entity entity, const YAML::Node& comp)
		{
			auto& component = entity.AddComponent<SpriteRendererComponent>();

			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			YamlUtils::Deserialize(comp, "Color", component.Color, Color::White);
			YamlUtils::Deserialize<AssetHandle>(comp, "Sprite", component.TextureHandle, INVALID_ASSET_HANDLE);
		};

		template<>
		static void Deserialize<Rigidbody2DComponent>(Entity entity, const YAML::Node& comp)
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
		static void Deserialize<BoxCollider2DComponent>(Entity entity, const YAML::Node& comp)
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
		static void Deserialize<CircleCollider2DComponent>(Entity entity, const YAML::Node& comp)
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
		std::string m_DebugName;
		std::string m_Source;
		DeserializerCallback m_Handler;
	};
}
