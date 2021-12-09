#pragma once

#include <hzrpch.h>
#include "WorldSerializer.h"
#include "Hazard/Utils/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::ECS::Loader {

	bool WorldSerializer::SerializeEditor(const char* file, Ref<World> world)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "World", !world->GetName().empty() ? world->GetName() : "Untitled world");

		YamlUtils::Sequence(out, "Entities", [&]() {
			world->GetWorldRegistry().each([&](auto entityID) {
				Entity entity{ entityID, world.Raw() };
				if (!entity)
					return;

				SerializeEntityEditor(entity, out);
				});
			});
		out << YAML::EndMap;

		std::ofstream fout(file);
		fout << out.c_str();
		fout.close();

		return true;
	}
	void WorldSerializer::SerializeEntityEditor(Entity& entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		YamlUtils::Serialize<uint32_t>(out, "Entity", entity);

		//General
		TrySerializeEditor<TagComponent>(entity, out);
		TrySerializeEditor<TransformComponent>(entity, out);
		TrySerializeEditor<CameraComponent>(entity, out);

		//Scripts
		TrySerializeEditor<ScriptComponent>(entity, out);
		TrySerializeEditor<VisualScriptComponent>(entity, out);

		//Lights
		TrySerializeEditor<SkyLightComponent>(entity, out);
		TrySerializeEditor<DirectionalLightComponent>(entity, out);
		TrySerializeEditor<PointLightComponent>(entity, out);

		//Audio
		TrySerializeEditor<AudioSourceComponent>(entity, out);

		//Meshes
		TrySerializeEditor<MeshComponent>(entity, out);
		TrySerializeEditor<SpriteRendererComponent>(entity, out);

		//Physics
		TrySerializeEditor<Rigidbody2DComponent>(entity, out);
		TrySerializeEditor<BoxCollider2DComponent>(entity, out);

		TrySerializeEditor<BatchComponent>(entity, out);

		out << YAML::EndMap;
	}

	template<typename T>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, T& component, YAML::Emitter& out) {
		static_assert(false, "Failed to serialize " + std::string(typeid(T).name()));
	}

	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, TagComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "TagComponent", [&]() {
			YamlUtils::Serialize(out, "Tag", component.Tag);
			});
	}

	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, TransformComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "TransformComponent", [&]() {
			YamlUtils::Serialize(out, "Translation", component.Translation);
			YamlUtils::Serialize(out, "Rotation", component.Rotation);
			YamlUtils::Serialize(out, "Scale", component.Scale);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, CameraComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "CameraComponent", [&]() {
			YamlUtils::Serialize(out, "Projection", component.GetProjectionType() ? "Orthographic" : "Perspective");
			YamlUtils::Serialize(out, "Fov", component.GetFov());
			YamlUtils::Serialize(out, "Clipping", component.GetClipping());
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, ScriptComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "ScriptComponent", [&]() {
			YamlUtils::Serialize(out, "ModuleName", component.ModuleName);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, VisualScriptComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "VisualScriptComponent", [&]() {
			YamlUtils::Serialize(out, "FileName", component.Filename);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, SkyLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "SkyLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, DirectionalLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "DirectionalLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, PointLightComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "PointLightComponent", [&]() {
			YamlUtils::Serialize(out, "Tint", component.Tint);
			YamlUtils::Serialize(out, "Intensity", component.Intensity);
			YamlUtils::Serialize(out, "Radius", component.Radius);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<AudioSourceComponent>(Entity& entity, AudioSourceComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "AudioSourceComponent", [&]() {
			YamlUtils::Serialize(out, "AudioFile", component.SourceFile);
			YamlUtils::Serialize(out, "Gain", component.Source.GetGain());
			YamlUtils::Serialize(out, "Looping", component.Source.IsLooping());
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor<MeshComponent>(Entity& entity, MeshComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "MeshComponent", [&]() {
			YamlUtils::Serialize(out, "File", "");
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, SpriteRendererComponent& component, YAML::Emitter& out)
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
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, Rigidbody2DComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "Rigidbody2DComponent", [&]() {
			YamlUtils::Serialize(out, "Type", Physics::BodyTypeToString(component.Type));
			YamlUtils::Serialize(out, "FixedRotation", component.FixedRotation);
			YamlUtils::Serialize(out, "UseGravity", component.UseGravity);
			});
	}
	template<>
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, BoxCollider2DComponent& component, YAML::Emitter& out)
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
	static void WorldSerializer::SerializeComponentEditor(Entity& entity, BatchComponent& component, YAML::Emitter& out)
	{
		YamlUtils::Map(out, "BatchRendererComponent", [&]() {
			YamlUtils::Serialize(out, "Size", component.Size);
			YamlUtils::Serialize(out, "Tint", component.Tint);
			});
	}

	bool WorldSerializer::SerializeRuntime(const char* file, World& scene)
	{
		return false;
	}
}