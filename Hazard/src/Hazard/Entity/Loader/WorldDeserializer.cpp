#pragma once

#include <hzrpch.h>
#include "WorldDeserializer.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "Hazard/File/YamlUtils.h"

namespace Hazard::ECS::Loader
{
	static WorldBackground StringToWorldType(const std::string& value) {
		if (value == "Sky")		return WorldBackground::Sky;
		if (value == "HDRI")	return WorldBackground::HDRI;
		return WorldBackground::Colored;
	}

	World* WorldDeserializer::DeserializeEditor(const char* file)
	{
		HZR_PROFILE_FUNCTION();
		YAML::Node root = YAML::LoadFile(file);
		World* world = new World(file);

		//Set scene name
		if (!root["World"])
			return world;

		world->SetName(root["World"].as<std::string>());

		if (root["Environment"])
		{
			auto env = root["Environment"];
			YamlUtils::Deserialize(env, "ClearColor", world->GetWorldData().renderer->m_Color);
			WorldBackground type = StringToWorldType(env["Type"].as<std::string>());

			std::string file = "";
			if (env["File"]) {
				YamlUtils::Deserialize(env, "File", file);
			}
			world->SetBackground(type, file);
			float gamma = 0;
			if (env["Gamma"])
				YamlUtils::Deserialize(env, "Gamma", gamma);
			world->GetWorldData().renderer->SetGamma(gamma);
		}

		//Loop entities
		auto entities = root["Entities"];
		if (entities) {

			for (int i = entities.size() - 1; i >= 0; i--) {

				auto node = entities[i];
				Entity entity = world->CreateEntity("");
				//Deserialize components
				TryDeserializeComponent<TagComponent>("TagComponent", entity, node);
				TryDeserializeComponent<TransformComponent>("TransformComponent", entity, node);
				TryDeserializeComponent<CameraComponent>("CameraComponent", entity, node);

				TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);
				TryDeserializeComponent<VisualScriptComponent>("VisualScriptComponent", entity, node);

				TryDeserializeComponent<SkyLightComponent>("SkyLightComponent", entity, node);
				TryDeserializeComponent<DirectionalLightComponent>("DirectionalLightComponent", entity, node);
				TryDeserializeComponent<PointLightComponent>("PointLightComponent", entity, node);

				TryDeserializeComponent<AudioSourceComponent>("AudioSourceComponent", entity, node);

				TryDeserializeComponent<MeshComponent>("MeshComponent", entity, node);
				TryDeserializeComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, node);

				TryDeserializeComponent<Rigidbody2DComponent>("Rigidbody2DComponent", entity, node);
				TryDeserializeComponent<BoxCollider2DComponent>("BoxCollider2DComponent", entity, node);

				TryDeserializeComponent<BatchComponent>("BatchComponent", entity, node);
			}
		}
		return world;
	}
	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node) {
		if (node[key]) {
			Deserialize<T>(entity, node[key]);
		}
	}
	template<typename T>
	void WorldDeserializer::Deserialize(Entity entity, YAML::Node node) {
		static_assert(false);
	}

	template<>
	static void WorldDeserializer::Deserialize<TagComponent>(Entity entity, YAML::Node comp) 
	{
		YamlUtils::Deserialize(comp, "Tag", entity.GetComponent<TagComponent>().m_Tag);
	};

	template<>
	static void WorldDeserializer::Deserialize<TransformComponent>(Entity entity, YAML::Node comp) {
		auto& c = entity.GetComponent<TransformComponent>();

		YamlUtils::Deserialize(comp, "Translation", c.m_Translation);
		YamlUtils::Deserialize(comp, "Rotation", c.m_Rotation);
		YamlUtils::Deserialize(comp, "Scale", c.m_Scale);
	};
	template<>
	static void WorldDeserializer::Deserialize<CameraComponent>(Entity entity, YAML::Node comp) {

		auto& c = entity.AddComponent<CameraComponent>();
		std::string projection;
		YamlUtils::Deserialize(comp, "Projection", projection);
		c.SetProjection(projection == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
		c.SetFov(comp["Fov"].as<float>());
		glm::vec2 clipping;
		YamlUtils::Deserialize(comp, "Clipping", clipping);
		c.SetZNear(clipping.x);
		c.SetZFar(clipping.y);
	};
	template<>
	static void WorldDeserializer::Deserialize<ScriptComponent>(Entity entity, YAML::Node comp) {
		std::string moduleName;
		YamlUtils::Deserialize(comp, "ModuleName", moduleName);
		entity.AddComponentWithCallback<ScriptComponent>([&](ScriptComponent& c) {
			c.m_ModuleName = moduleName;
			});
	};
	template<>
	static void WorldDeserializer::Deserialize<VisualScriptComponent>(Entity entity, YAML::Node comp) 
	{
		std::string filename;
		YamlUtils::Deserialize(comp, "FileName", filename);

		entity.AddComponentWithCallback<VisualScriptComponent>([&](VisualScriptComponent& comp) {
			comp.m_Filename = filename;
			});
	};
	template<>
	static void WorldDeserializer::Deserialize<SkyLightComponent>(Entity entity, YAML::Node comp) 
	{
		auto& c = entity.AddComponent<SkyLightComponent>();
		YamlUtils::Deserialize(comp, "Tint", c.m_Tint);
		YamlUtils::Deserialize(comp, "Intensity", c.m_Intensity);
	}
	template<>
	static void WorldDeserializer::Deserialize<DirectionalLightComponent>(Entity entity, YAML::Node comp) 
	{
		auto& c = entity.AddComponent<DirectionalLightComponent>();
		YamlUtils::Deserialize(comp, "Tint", c.m_Tint);
		YamlUtils::Deserialize(comp, "Intensity", c.m_Intensity);
	}
	template<>
	static void WorldDeserializer::Deserialize<PointLightComponent>(Entity entity, YAML::Node comp) 
	{
		auto& c = entity.AddComponent<PointLightComponent>();
		YamlUtils::Deserialize(comp, "Tint", c.m_Tint);
		YamlUtils::Deserialize(comp, "Intensity", c.m_Intensity);
		YamlUtils::Deserialize(comp, "Radius", c.m_Radius);
	}
	template<>
	static void WorldDeserializer::Deserialize<AudioSourceComponent>(Entity entity, YAML::Node comp) {
		AudioSourceComponent& source = entity.AddComponentWithCallback<AudioSourceComponent>([&](AudioSourceComponent& c) {
			YamlUtils::Deserialize(comp, "AudioFile", c.sourceFile);
		});

		if (comp["Gain"]) {
			source.source.SetGain(comp["Gain"].as<float>());
		}

		if (comp["Looping"]) {
			source.source.SetLoop(comp["Looping"].as<std::string>() == "True");
		}
	};
	template<>
	static void WorldDeserializer::Deserialize<MeshComponent>(Entity entity, YAML::Node comp) {
		auto& c = entity.AddComponent<MeshComponent>();
		std::string fileName;
		YamlUtils::Deserialize(comp, "File", fileName);
		c.m_Mesh = Rendering::MeshFactory::LoadMesh(fileName);

		Ref<Rendering::GraphicsPipeline> pipeline = Vault::Get<Rendering::GraphicsPipeline>("DefaultMeshShader");
		c.m_Material->SetPipeline(pipeline);
	};
	template<>
	static void WorldDeserializer::Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp) {
		auto& component = entity.AddComponent<SpriteRendererComponent>();

		YamlUtils::Deserialize(comp, "Tint", component.m_Tint);
		if (comp["Texture"]) {

			using namespace Hazard::Rendering;

			Texture2DCreateInfo createInfo;
			YamlUtils::Deserialize(comp, "Texture", createInfo.filename);
			component.m_Texture = RenderUtils::Create<Texture2D>(createInfo);
		}
	};
	template<>
	static void WorldDeserializer::Deserialize<Rigidbody2DComponent>(Entity entity, YAML::Node comp)
	{
		auto& component = entity.AddComponent<Rigidbody2DComponent>();
		std::string bodyType;
		YamlUtils::Deserialize(comp, "Type", bodyType);
		component.type = StringToBodyType(bodyType);
		YamlUtils::Deserialize(comp, "FixedRotation", component.FixedRotation);
		YamlUtils::Deserialize(comp, "UseGravity", component.UseGravity);
	}
	template<>
	static void WorldDeserializer::Deserialize<BoxCollider2DComponent>(Entity entity, YAML::Node comp)
	{
		auto& component = entity.AddComponent<BoxCollider2DComponent>();
		YamlUtils::Deserialize(comp, "Offset", component.Offset);
		YamlUtils::Deserialize(comp, "Size", component.Size);
		YamlUtils::Deserialize(comp, "Density", component.Density);
		YamlUtils::Deserialize(comp, "Friction", component.Friction);
		YamlUtils::Deserialize(comp, "Restitution", component.Restitution);
		YamlUtils::Deserialize(comp, "RestitutionThreshold", component.RestitutionThreshold);
		YamlUtils::Deserialize(comp, "IsSensor", component.IsSensor);
	}
	template<>
	static void WorldDeserializer::Deserialize<BatchComponent>(Entity entity, YAML::Node comp)
	{
		auto& component = entity.AddComponent<BatchComponent>();
		YamlUtils::Deserialize(comp, "Size", component.m_Size);
		YamlUtils::Deserialize(comp, "Tint", component.m_Tint);
	}
}