#pragma once

#include "../WorldHandler.h"
#include "../World.h"
#include "../Entity.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "WorldSerializer.h"
#include "Hazard/Utils/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Hazard::ECS::Loader 
{
	class WorldSerializer {
	public:

		template<typename T>
		static void TrySerializeEditor(Entity& entity, YAML::Emitter& out)
		{
			if (entity.HasComponent<T>()) {
				SerializeComponentEditor(entity, entity.GetComponent<T>(), out);
			}
		}
		static bool SerializeEditor(const char* file, Ref<World> world);
		static void SerializeEntityEditor(Entity& entity, YAML::Emitter& out);
		template<typename T>
		static void SerializeComponentEditor(Entity& entity, T& component, YAML::Emitter& out);


		static bool SerializeRuntime(const char* file, World& scene);


	template<>
	static void SerializeComponentEditor(Entity& entity, TagComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "TagComponent", [&]() {
			YamlUtils::Serialize(out, "Tag", component.Tag);
			});
	}

	template<>
	static void SerializeComponentEditor(Entity& entity, TransformComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "TransformComponent", [&]() {
			YamlUtils::Serialize(out, "Translation", component.Translation);
			YamlUtils::Serialize(out, "Rotation", component.Rotation);
			YamlUtils::Serialize(out, "Scale", component.Scale);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, CameraComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "CameraComponent", [&]() {
			YamlUtils::Serialize(out, "Projection", component.GetProjectionType() ? "Orthographic" : "Perspective");
			YamlUtils::Serialize(out, "Fov", component.GetFov());
			YamlUtils::Serialize(out, "Clipping", component.GetClipping());
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, ScriptComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "ScriptComponent", [&]() {
			YamlUtils::Serialize(out, "ModuleName", component.ModuleName);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, VisualScriptComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "VisualScriptComponent", [&]() {
			YamlUtils::Serialize(out, "FileName", component.Filename);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, SkyLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "SkyLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, DirectionalLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "DirectionalLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, PointLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "PointLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			YamlUtils::Serialize(out, "Radius", component.Radius);
			});
	}
	template<>
	static void SerializeComponentEditor<AudioSourceComponent>(Entity& entity, AudioSourceComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "AudioSourceComponent", [&]() {
			YamlUtils::Serialize(out, "AudioFile", component.SourceFile);
			YamlUtils::Serialize(out, "Gain", component.Gain);
			YamlUtils::Serialize(out, "Pitch", component.Pitch);
			YamlUtils::Serialize(out, "Looping", component.Looping);
			YamlUtils::Serialize(out, "Spatial", component.Spatial);
			YamlUtils::Serialize(out, "PlayOnCreate", component.PlayOnCreate);
			});
	}
	template<>
	static void SerializeComponentEditor<MeshComponent>(Entity& entity, MeshComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "MeshComponent", [&]() {
			YamlUtils::Serialize(out, "File", "");
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, SpriteRendererComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "SpriteRendererComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			if (component.Texture->IsValid()) {
				AssetMetadata& meta = component.Texture->GetMetadata();
				YamlUtils::Serialize(out, "Texture", meta.Path.string());
			}
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, Rigidbody2DComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "Rigidbody2DComponent", [&]() {
			YamlUtils::Serialize(out, "Type", Physics::BodyTypeToString(component.Type));
			YamlUtils::Serialize(out, "FixedRotation", component.FixedRotation);
			YamlUtils::Serialize(out, "UseGravity", component.UseGravity);
			});
	}
	template<>
	static void SerializeComponentEditor(Entity& entity, BoxCollider2DComponent& component, YAML::Emitter& out)
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
	static void SerializeComponentEditor(Entity& entity, CircleCollider2DComponent& component, YAML::Emitter& out)
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
	static void SerializeComponentEditor(Entity& entity, BatchComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "BatchRendererComponent", [&]() {
			YamlUtils::Serialize(out, "Size", component.Size);
			YamlUtils::Serialize(out, "Tint", component.Tint);
			});
	}

	};
}