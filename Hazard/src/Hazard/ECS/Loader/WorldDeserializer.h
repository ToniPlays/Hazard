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
	using DeserializerProgressHandler = std::function<void(Entity&, size_t, size_t)>;

	class WorldDeserializer {
	public:

		WorldDeserializer() = default;

		Ref<World> DeserializeEditor(const std::filesystem::path& file, uint32_t flags = 0);
		Ref<World> DeserializeRuntime(const std::filesystem::path& file, uint32_t flags = 0) { return nullptr; };

		std::vector<JobPromise>& GetPromises() { return m_Promises; }


		void SetProgressHandler(DeserializerProgressHandler handler)
		{
			m_Handler = handler;
		}

		template<typename T>
		void TryDeserializeComponent(const char* key, Entity entity, YAML::Node node);
		template<typename T>
		void Deserialize(Entity entity, YAML::Node node);

		template<>
		void Deserialize<TagComponent>(Entity entity, YAML::Node comp)
		{
			YamlUtils::Deserialize(comp, "Tag", entity.GetComponent<TagComponent>().Tag, std::string("New entity"));
		};

		template<>
		void Deserialize<TransformComponent>(Entity entity, YAML::Node comp) {
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
		void Deserialize<CameraComponent>(Entity entity, YAML::Node comp) {

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
		void Deserialize<ScriptComponent>(Entity entity, YAML::Node comp) {
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
		void Deserialize<SkyLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<SkyLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "SkyboxLod", c.LodLevel, 0.0f);

			AssetHandle handle;
			YamlUtils::Deserialize<AssetHandle>(comp, "EnvironmentMap", handle, INVALID_ASSET_HANDLE);

			if (handle == INVALID_ASSET_HANDLE) return;

			if (m_CanAsync)
			{
				TypedJobPromise<Ref<Texture2DAsset>> promise = AssetManager::GetAssetAsync<Texture2DAsset>(handle);
				promise.Then([e = entity](JobSystem* system, Job* job) -> size_t {

					Entity entity = { e };
					Ref<Texture2DAsset> asset = *system->GetJob(job->Dependency)->Value<Ref<Texture2DAsset>>();
					entity.GetComponent<SkyLightComponent>().EnvironmentMap = EnvironmentMap::Create(asset);

					return 0;
					});
				m_Promises.push_back(promise);
			}
			else
			{
				Ref<Texture2DAsset> sourceImage = AssetManager::GetAsset<Texture2DAsset>(handle);
				c.EnvironmentMap = EnvironmentMap::Create(sourceImage);
			}
		}

		template<>
		void Deserialize<DirectionalLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<DirectionalLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
		}
		template<>
		void Deserialize<PointLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<PointLightComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Color", c.LightColor, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "Radius", c.Radius, 1.0f);
		}
		template<>
		void Deserialize<MeshComponent>(Entity entity, YAML::Node comp) {
			auto& c = entity.AddComponent<MeshComponent>();
			YamlUtils::Deserialize(comp, "Active", c.Active, true);
			YamlUtils::Deserialize(comp, "Metalness", c.Metalness, 0.0f);
			YamlUtils::Deserialize(comp, "Roughness", c.Roughness, 1.0f);

			AssetHandle handle;
			YamlUtils::Deserialize<AssetHandle>(comp, "Mesh", handle, INVALID_ASSET_HANDLE);

			if (handle == INVALID_ASSET_HANDLE)
				return;

			if (m_CanAsync)
			{
				TypedJobPromise<Ref<Mesh>> promise = AssetManager::GetAssetAsync<Mesh>(handle);
				promise.Then([e = entity](JobSystem* system, Job* job) -> size_t {
					Entity entity = { e };
					Ref<Mesh> asset = *system->GetJob(job->Dependency)->Value<Ref<Mesh>>();
					entity.GetComponent<MeshComponent>().m_MeshHandle = asset;
					return 0;
					});
				m_Promises.push_back(promise);
			}
			else
			{
				c.m_MeshHandle = AssetManager::GetAsset<Mesh>(handle);
			}
		};
		template<>
		void Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp) {
			auto& component = entity.AddComponent<SpriteRendererComponent>();

			YamlUtils::Deserialize(comp, "Active", component.Active, true);
			YamlUtils::Deserialize(comp, "Color", component.Color, Color::White);

			if (!comp["Sprite"]) return;

			AssetHandle handle = INVALID_ASSET_HANDLE;
			YamlUtils::Deserialize<AssetHandle>(comp, "Sprite", handle, INVALID_ASSET_HANDLE);
			component.Texture = AssetManager::GetAsset<Texture2DAsset>(handle);
		};
		template<>
		void Deserialize<Rigidbody2DComponent>(Entity entity, YAML::Node comp)
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
		void Deserialize<BoxCollider2DComponent>(Entity entity, YAML::Node comp)
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
		void Deserialize<CircleCollider2DComponent>(Entity entity, YAML::Node comp)
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
		bool m_CanAsync = false;
		std::vector<JobPromise> m_Promises;

		DeserializerProgressHandler m_Handler;
	};
}
