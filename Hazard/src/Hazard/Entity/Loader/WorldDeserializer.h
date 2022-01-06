#pragma once

#include "../Entity.h"
#include <yaml-cpp/yaml.h>
#include "Hazard/Utils/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::ECS::Loader 
{
	class WorldDeserializer {
	public:

        WorldDeserializer() = default;
        
        Ref<World> DeserializeEditor(const std::string& file);
        Ref<World> DeserializeRuntime(const std::string& file) { return nullptr; };

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

			YamlUtils::Deserialize(comp, "Translation", c.Translation, glm::vec3(0.0f));
			YamlUtils::Deserialize(comp, "Rotation", c.Rotation, glm::vec3(0.0f));
			YamlUtils::Deserialize(comp, "Scale", c.Scale, glm::vec3(1.0f));
		};
		template<>
		void Deserialize<CameraComponent>(Entity entity, YAML::Node comp) {

			auto& c = entity.AddComponent<CameraComponent>();
			std::string projection;
			YamlUtils::Deserialize(comp, "Projection", projection, std::string("Perspective"));
			c.SetProjection(projection == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
			c.SetFov(comp["Fov"].as<float>());

			glm::vec2 clipping;
			YamlUtils::Deserialize(comp, "Clipping", clipping, glm::vec2(0.03f, 1.0f));
			c.SetZNear(clipping.x);
			c.SetZFar(clipping.y);
		};
		template<>
		void Deserialize<ScriptComponent>(Entity entity, YAML::Node comp) {
			std::string moduleName;
			YamlUtils::Deserialize(comp, "ModuleName", moduleName, std::string(""));

			entity.AddComponentWithCallback<ScriptComponent>([&](ScriptComponent& c) {
				c.ModuleName = moduleName;
				});
		};
		template<>
		void Deserialize<VisualScriptComponent>(Entity entity, YAML::Node comp)
		{
			std::string filename;
			YamlUtils::Deserialize(comp, "FileName", filename, std::string(""));

			entity.AddComponentWithCallback<VisualScriptComponent>([&](VisualScriptComponent& comp) {
				comp.Filename = filename;
				});
		};
		template<>
		void Deserialize<SkyLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<SkyLightComponent>();
			YamlUtils::Deserialize(comp, "Tint", c.Tint, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
		}
		template<>
		void Deserialize<DirectionalLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<DirectionalLightComponent>();
			YamlUtils::Deserialize(comp, "Tint", c.Tint, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
		}
		template<>
		void Deserialize<PointLightComponent>(Entity entity, YAML::Node comp)
		{
			auto& c = entity.AddComponent<PointLightComponent>();
			YamlUtils::Deserialize(comp, "Tint", c.Tint, Color::White);
			YamlUtils::Deserialize(comp, "Intensity", c.Intensity, 1.0f);
			YamlUtils::Deserialize(comp, "Radius", c.Radius, 1.0f);
		}
		template<>
		void Deserialize<AudioSourceComponent>(Entity entity, YAML::Node comp) {
			AudioSourceComponent& source = entity.AddComponent<AudioSourceComponent>();

			YamlUtils::Deserialize(comp, "AudioFile", source.SourceFile, std::string(""));
			AssetHandle handle = AssetManager::ImportAsset(source.SourceFile);

			if (handle != INVALID_ASSET_HANDLE) {
				Ref<Audio::AudioBufferData> buffer = AssetManager::GetAsset<Audio::AudioBufferData>(handle);
				source.Source.SetSourceBuffer(buffer);
			}

			YamlUtils::Deserialize(comp, "AudioFile", source.SourceFile, std::string(""));
			YamlUtils::Deserialize(comp, "Gain", source.Gain, 1.0f);
			YamlUtils::Deserialize(comp, "Pitch", source.Pitch, 1.0f);
			YamlUtils::Deserialize(comp, "Looping", source.Looping, false);
			YamlUtils::Deserialize(comp, "Spatial", source.Spatial, false);
			YamlUtils::Deserialize(comp, "PlayOnCreate", source.PlayOnCreate, false);
		};
		template<>
		void Deserialize<MeshComponent>(Entity entity, YAML::Node comp) {
			auto& c = entity.AddComponent<MeshComponent>();
			std::string fileName;
			YamlUtils::Deserialize(comp, "File", fileName, std::string(""));
			if (fileName.empty()) return;

			AssetHandle handle = AssetManager::ImportAsset(fileName);
			c.m_Mesh = AssetManager::GetAsset<Rendering::Mesh>(handle);
		};
		template<>
		void Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp) {
			auto& component = entity.AddComponent<SpriteRendererComponent>();

			YamlUtils::Deserialize(comp, "Tint", component.Tint, Color::White);
			if (comp["Texture"]) {
				using namespace Hazard::Rendering;
				std::string fileName;
				YamlUtils::Deserialize(comp, "Texture", fileName, std::string(""));
				AssetHandle handle = AssetManager::ImportAsset(fileName);
				if(handle != INVALID_ASSET_HANDLE)
					component.Texture = AssetManager::GetAsset<Texture2D>(handle);
			}
		};
		template<>
		void Deserialize<Rigidbody2DComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<Rigidbody2DComponent>();
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
			YamlUtils::Deserialize(comp, "Offset", component.Offset, glm::vec2(0.0f));
			YamlUtils::Deserialize(comp, "Radius", component.Radius, 0.5f);
			YamlUtils::Deserialize(comp, "Density", component.Density, 1.0f);
			YamlUtils::Deserialize(comp, "Friction", component.Friction, 1.0f);
			YamlUtils::Deserialize(comp, "Restitution", component.Restitution, 1.0f);
			YamlUtils::Deserialize(comp, "RestitutionThreshold", component.RestitutionThreshold, 0.1f);
			YamlUtils::Deserialize(comp, "IsSensor", component.IsSensor, false);
		}
		template<>
		void Deserialize<BatchComponent>(Entity entity, YAML::Node comp)
		{
			auto& component = entity.AddComponent<BatchComponent>();
			YamlUtils::Deserialize(comp, "Size", component.Size, (uint32_t)1);
			YamlUtils::Deserialize(comp, "Tint", component.Tint, Color::White);
		}
	};
}
