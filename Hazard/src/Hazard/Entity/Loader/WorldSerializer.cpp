
#include <hzrpch.h>
#include "WorldSerializer.h"
#include "Hazard/Utils/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::ECS::Loader {

	bool WorldSerializer::SerializeEditor(const std::string& file)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "World", !m_World->GetName().empty() ? m_World->GetName() : "Untitled world");

		YamlUtils::Sequence(out, "Entities", [&]() {
            m_World->GetWorldRegistry().each([&](auto entityID) {
				Entity entity{ entityID, m_World.Raw() };

				HZR_GUARD(entity);
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
		TrySerializeEditor<CircleCollider2DComponent>(entity, out);

		TrySerializeEditor<BatchComponent>(entity, out);

		out << YAML::EndMap;
	}

	bool WorldSerializer::SerializeRuntime(const std::string& file)
	{
		return false;
	}
}
