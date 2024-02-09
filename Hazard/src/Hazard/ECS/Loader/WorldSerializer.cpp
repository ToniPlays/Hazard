
#include <hzrpch.h>
#include "WorldSerializer.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard
{
	std::string WorldSerializer::Serialize()
	{
		HZR_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "World", "Untitled world");

		YamlUtils::Sequence(out, "Entities", [&]() {
			m_World->GetWorldRegistry().each([&](auto entityID) {
				Entity entity{ entityID, m_World.Raw() };
				if (entity)
					SerializeEntity(entity, out);
			});
		});

		out << YAML::EndMap;

		return out.c_str();
	}

	void WorldSerializer::SerializeEntity(Entity& entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		YamlUtils::Serialize<uint64_t>(out, "Entity", entity.GetUID());

		//General
		TrySerialize<TagComponent>(entity, out);
		TrySerialize<TransformComponent>(entity, out);
		TrySerialize<CameraComponent>(entity, out);

		//Scripts
		TrySerialize<ScriptComponent>(entity, out);

		//Lights
		TrySerialize<SkyLightComponent>(entity, out);
		TrySerialize<DirectionalLightComponent>(entity, out);
		TrySerialize<PointLightComponent>(entity, out);

		//Meshes
		TrySerialize<MeshComponent>(entity, out);
		TrySerialize<SpriteRendererComponent>(entity, out);

		//Physics
		TrySerialize<Rigidbody2DComponent>(entity, out);
		TrySerialize<BoxCollider2DComponent>(entity, out);
		TrySerialize<CircleCollider2DComponent>(entity, out);

		out << YAML::EndMap;
	}
}
