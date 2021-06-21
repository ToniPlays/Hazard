#pragma once

#include <hzrpch.h>
#include "WorldSerializer.h"


namespace Hazard::ECS::Loader {

	static std::string BackgroundTypeToString(WorldBackground type) {
		switch (type)
		{
		case Hazard::ECS::Sky:	return "Sky";
		case Hazard::ECS::HDRI:	return "HDRI";
		}
		return "Color";
	}

	bool WorldSerializer::SerializeEditor(const char* file, World& world)
	{
		HZR_PROFILE_FUNCTION();
		YAML::Emitter out;

		WorldData& worldData = world.GetWorldData();

		out << YAML::BeginMap;
		out << YAML::Key << "World" << YAML::Value << (!world.GetName().empty() ? world.GetName() : "Untitled world");
		out << YAML::Key << "Environment" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "ClearColor" << YAML::Value; Convert(out, worldData.renderer->m_Color);
		out << YAML::Key << "Type" << YAML::Value << BackgroundTypeToString(worldData.background);
		out << YAML::Key << "Gamma" << YAML::Value << worldData.renderer->GetGamma();
		out << YAML::Key << "File" << YAML::Value << worldData.renderer->GetFile();

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
	void WorldSerializer::SerializeEntityEditor(Entity& entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		TrySerializeEditor<TagComponent>(entity, out);
		TrySerializeEditor<TransformComponent>(entity, out);
		TrySerializeEditor<SpriteRendererComponent>(entity, out);
		TrySerializeEditor<CameraComponent>(entity, out);
		TrySerializeEditor<ScriptComponent>(entity, out);
		TrySerializeEditor<MeshComponent>(entity, out);
		TrySerializeEditor<AudioSourceComponent>(entity, out);

		out << YAML::EndMap;
	}

	template<typename T>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, T& component, YAML::Emitter& out) {
		static_assert(false, "Failed to serialize " + std::string(typeid(T).name()));
	}

	template<>
	static void WorldSerializer::SerializeComponentEditor<TagComponent>(Entity& entity, TagComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Tag" << YAML::Value << component.m_Tag;
		out << YAML::EndMap;
	}

	template<>
	static void WorldSerializer::SerializeComponentEditor<TransformComponent>(Entity& entity, TransformComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Translation" << YAML::Value; Convert(out, component.m_Translation);
		out << YAML::Key << "Rotation" << YAML::Value; Convert(out, component.m_Rotation);
		out << YAML::Key << "Scale" << YAML::Value; Convert(out, component.m_Scale);
		out << YAML::EndMap;

	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Tint" << YAML::Value; Convert(out, (glm::vec4)component.m_Tint);

		if (component.m_Texture) {
			if (component.m_Texture->GetData().file != "White")
				out << YAML::Key << "Texture" << YAML::Value << std::string(component.m_Texture->GetData().file);
		}
		out << YAML::EndMap;
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<CameraComponent>(Entity& entity, CameraComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Projection" << YAML::Value << (component.GetProjectionType() ? "Orthographic" : "Perspective");
		out << YAML::Key << "Fov" << YAML::Value << component.GetFov();
		out << YAML::Key << "Clipping" << YAML::Value; Convert(out, component.GetClipping());
		out << YAML::EndMap;
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<ScriptComponent>(Entity& entity, ScriptComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "ScriptComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "ModuleName" << YAML::Value << component.m_ModuleName;
		out << YAML::EndMap;
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<MeshComponent>(Entity& entity, MeshComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
		std::string file = component.m_Mesh ? component.m_Mesh->GetFile() : "";
		out << YAML::Key << "File" << YAML::Value << file;
		out << YAML::EndMap;
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<AudioSourceComponent>(Entity& entity, AudioSourceComponent& component, YAML::Emitter& out)
	{
		out << YAML::Key << "AudioSourceComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "AudioFile" << YAML::Value << component.sourceFile;
		out << YAML::Key << "Gain" << YAML::Value << component.source.GetGain();
		out << YAML::Key << "Looping" << YAML::Value; Convert(out, component.source.IsLooping());
		out << YAML::EndMap;
	}

	bool WorldSerializer::SerializeRuntime(const char* file, World& scene)
	{
		return false;
	}
}