#pragma once

#include <hzrpch.h>
#include "SceneSerializer.h"


namespace Hazard::ECS::Loader {

	static std::string BackgroundTypeToString(WorldBackground type) {
		switch (type)
		{
		case Hazard::ECS::Sky:	return "Sky";
		case Hazard::ECS::HDRI:	return "HDRI";
		}
		return "Color";
	}

	bool SceneSerializer::SerializeEditor(const char* file, World& world)
	{
		HZR_PROFILE_FUNCTION();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << (world.GetName() != "" ? world.GetName() : "Untitled scene");
		out << YAML::Key << "Environment" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "ClearColor" << YAML::Value; Convert(out, world.GetWorldData().renderer->m_Color);
		out << YAML::Key << "Type" << YAML::Value << BackgroundTypeToString(world.GetWorldData().background);
		out << YAML::Key << "File" << YAML::Value << world.GetWorldData().renderer->GetFile();

		out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		world.GetWorldRegistry().each([&](auto entityID) {
			Entity entity{ entityID, &world };
			if (!entity) {
				return;
			}
			SerializeEntityEditor(entity, out);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(file);
		fout << out.c_str();

		return true;
	}
	void SceneSerializer::SerializeEntityEditor(Entity entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		TrySerializeEditor<TagComponent>(entity, out);
		TrySerializeEditor<TransformComponent>(entity, out);
		TrySerializeEditor<SpriteRendererComponent>(entity, out);
		TrySerializeEditor<CameraComponent>(entity, out);
		TrySerializeEditor<ScriptComponent>(entity, out);
		TrySerializeEditor<MeshComponent>(entity, out);

		out << YAML::EndMap;
	}

	template<typename T>
	static void SceneSerializer::SerializeComponentEditor(Entity entity, T& component, YAML::Emitter& out) {
		static_assert(false, "Failed to serialize " + std::string(typeid(T).name()));
	}

	template<>
	static void SceneSerializer::SerializeComponentEditor<TagComponent>(Entity entity, TagComponent& component, YAML::Emitter& out)
	{
		if (!entity.HasComponent<TagComponent>()) return;
		auto tag = entity.GetComponent<TagComponent>().m_Tag;

		out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Tag" << YAML::Value << tag;
		out << YAML::EndMap;
	}

	template<>
	static void SceneSerializer::SerializeComponentEditor<TransformComponent>(Entity entity, TransformComponent& component, YAML::Emitter& out)
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
	static void SceneSerializer::SerializeComponentEditor<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component, YAML::Emitter& out)
	{
		if (!entity.HasComponent<SpriteRendererComponent>()) return;
		auto& c = entity.GetComponent<SpriteRendererComponent>();

		out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Tint" << YAML::Value; Convert(out, (glm::vec4)c.m_Tint);

		if (c.m_Texture) {
			if (c.m_Texture->GetData().file != "White")
				out << YAML::Key << "Texture" << YAML::Value << std::string(c.m_Texture->GetData().file);
		}
		out << YAML::EndMap;
	}
	template<>
	static void SceneSerializer::SerializeComponentEditor<CameraComponent>(Entity entity, CameraComponent& component, YAML::Emitter& out)
	{
		if (!entity.HasComponent<CameraComponent>()) return;
		auto& c = entity.GetComponent<CameraComponent>();
		out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Projection" << YAML::Value << (c.GetProjectionType() ? "Orthographic" : "Perspective");
		out << YAML::Key << "Fov" << YAML::Value << c.GetFov();
		out << YAML::Key << "Clipping" << YAML::Value; Convert(out, c.GetClipping());
		out << YAML::EndMap;
	}
	template<>
	static void SceneSerializer::SerializeComponentEditor<ScriptComponent>(Entity entity, ScriptComponent& component, YAML::Emitter& out)
	{
		if (!entity.HasComponent<ScriptComponent>()) return;
		auto& c = entity.GetComponent<ScriptComponent>();
		out << YAML::Key << "ScriptComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "ModuleName" << YAML::Value << c.m_ModuleName;
		out << YAML::EndMap;
	}
	template<>
	static void SceneSerializer::SerializeComponentEditor<MeshComponent>(Entity entity, MeshComponent& component, YAML::Emitter& out)
	{
		if (!entity.HasComponent<MeshComponent>()) return;
		auto& c = entity.GetComponent<MeshComponent>();
		out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "File" << YAML::Value << c.m_Mesh->GetFile();
		out << YAML::EndMap;
	}

	bool SceneSerializer::SerializeRuntime(const char* file, Scene& scene)
	{
		return false;
	}
}