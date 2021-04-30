#pragma once

#include "SceneHandler.h"
#include "../World.h"
#include "../Entity.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

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


namespace Hazard::ECS::Loader {

	template<typename T>
	static void Convert(YAML::Emitter& out, T v) {
		static_assert(false);
	};

	template<>
	static void Convert(YAML::Emitter& out, glm::vec3 v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	};
	template<>
	static void Convert(YAML::Emitter& out, glm::vec4 v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	};

	class SceneSerializer {
	public:

		//Deserialize Editor
		static World* DeserializeEditor(const char* file);
		template<typename T>
		static void Deserialize(Entity entity, YAML::Node node);
		template<typename T>
		static void TryDeserialize(const char* key, Entity entity, YAML::Node node) 
		{
			if (node[key]) {
				Deserialize<T>(entity, node[key]);
			}
		}

		template<>
		static void Deserialize<TagComponent>(Entity entity, YAML::Node comp) {
			std::string tag = comp["Tag"].as<std::string>();
			entity.GetComponent<TagComponent>().m_Tag = tag;
		};

		template<>
		static void Deserialize<TransformComponent>(Entity entity, YAML::Node comp) {
			auto& c = entity.GetComponent<TransformComponent>();

			c.m_Translation = comp["Translation"].as<glm::vec3>();
			c.m_Rotation = comp["Rotation"].as<glm::vec3>();
			c.m_Scale = comp["Scale"].as<glm::vec3>();
		};
		template<>
		static void Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp) {
			auto& component = entity.AddComponent<SpriteRendererComponent>();
			component.m_Tint = Color::FromGLM(comp["Tint"].as<glm::vec4>());

			if (comp["Texture"]) {
				component.m_Texture = Rendering::RenderUtils::Create<Rendering::Texture2D>(comp["Texture"].as<std::string>().c_str());
			}
		};
		template<>
		static void Deserialize<CameraComponent>(Entity entity, YAML::Node comp) {

			auto& c = entity.AddComponent<CameraComponent>();
			c.SetProjection(comp["Projection"].as<std::string>() == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
			c.SetFov(comp["Fov"].as<float>());
		};

		template<>
		static void Deserialize<ScriptComponent>(Entity entity, YAML::Node comp) {
			std::string moduleName = comp["ModuleName"].as<std::string>();
			entity.AddComponentWithCallback<ScriptComponent>([&](ScriptComponent& comp) {
				comp.m_ModuleName = moduleName;
			});
		};
		template<>
		static void Deserialize<VisualScriptComponent>(Entity entity, YAML::Node comp) {
			std::string filename = comp["ModuleName"].as<std::string>();
			entity.AddComponentWithCallback<VisualScriptComponent>([&](VisualScriptComponent& comp) {
				comp.m_Filename = filename;
			});
		};
		template<>
		static void Deserialize<MeshComponent>(Entity entity, YAML::Node comp) {
			auto& c = entity.AddComponent<MeshComponent>();
			c.m_Mesh = Rendering::MeshFactory::LoadMesh(comp["File"].as<std::string>());
		};

		//Deserialize runtime
		static Scene* DeserializeRuntime(const char* file);

		//Serialize
		//Deserialize Editor

		template<typename T>
		static void TryDeserializeEditor(Entity entity, YAML::Emitter& out)
		{
			if (entity.HasComponent<T>()) {
				SerializeComponentEditor(entity, entity.GetComponent<T>(), out);
			}
		}
		static bool SerializeEditor(const char* file, World& world);
		static void SerializeEntityEditor(Entity entity, YAML::Emitter& out);
		template<typename T>
		static void SerializeComponentEditor(Entity entity, T& component, YAML::Emitter& out) {};

		template<>
		static void SerializeComponentEditor<TagComponent>(Entity entity, TagComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<TagComponent>()) return;
			auto tag = entity.GetComponent<TagComponent>().m_Tag;

			out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		template<>
		static void SerializeComponentEditor<TransformComponent>(Entity entity, TransformComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<TransformComponent>()) return;
			auto& c = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value; Convert(out, c.m_Translation);
			out << YAML::Key << "Rotation" << YAML::Value; Convert(out, c.m_Rotation);
			out << YAML::Key << "Scale" << YAML::Value; Convert(out, c.m_Scale);

			out << YAML::EndMap;

		}
		template<>
		static void SerializeComponentEditor<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<SpriteRendererComponent>()) return;
			auto& c = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Tint" << YAML::Value; Convert(out, (glm::vec4)c.m_Tint);

			if (c.m_Texture != nullptr) {
				if(std::string(c.m_Texture->GetFile()) != "White")
					out << YAML::Key << "Texture" << YAML::Value << std::string(c.m_Texture->GetFile());
			}
			out << YAML::EndMap;
		}
		template<>
		static void SerializeComponentEditor<CameraComponent>(Entity entity, CameraComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<CameraComponent>()) return;
			auto& c = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Projection" << YAML::Value << (c.GetProjectionType() ? "Orthographic" : "Perspective");
			out << YAML::Key << "Fov" << YAML::Value << c.GetFov();
			out << YAML::EndMap;
		}
		template<>
		static void SerializeComponentEditor<ScriptComponent>(Entity entity, ScriptComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<ScriptComponent>()) return;
			auto& c = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "ScriptComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "ModuleName" << YAML::Value << c.m_ModuleName;
			out << YAML::EndMap;
		}
		template<>
		static void SerializeComponentEditor<MeshComponent>(Entity entity, MeshComponent& component, YAML::Emitter& out)
		{
			if (!entity.HasComponent<MeshComponent>()) return;
			auto& c = entity.GetComponent<MeshComponent>();
			out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "File" << YAML::Value << c.m_Mesh->GetFile();
			out << YAML::EndMap;
		}

		static bool SerializeRuntime(const char* file, Scene& scene);
	};
}