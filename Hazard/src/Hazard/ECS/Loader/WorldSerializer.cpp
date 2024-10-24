
#include <hzrpch.h>
#include "WorldSerializer.h"
#include "Utility/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard {

	bool WorldSerializer::SerializeEditor(const std::filesystem::path& file)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_INFO("Saving editor world {0}", file.string());
		YAML::Emitter out;
		out << YAML::BeginMap;
		YamlUtils::Serialize(out, "World", !m_World->GetName().empty() ? m_World->GetName() : "Untitled world");

		YamlUtils::Sequence(out, "Entities", [&]() {
			m_World->GetWorldRegistry().each([&](auto entityID) {
				Entity entity{ entityID, m_World.Raw() };
				if (entity)
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
		YamlUtils::Serialize<uint64_t>(out, "Entity", entity.GetUID());

		//General
		TrySerializeEditor<TagComponent>(entity, out);
		TrySerializeEditor<TransformComponent>(entity, out);
		TrySerializeEditor<CameraComponent>(entity, out);

		//Scripts
		TrySerializeEditor<ScriptComponent>(entity, out);

		//Lights
		TrySerializeEditor<SkyLightComponent>(entity, out);
		TrySerializeEditor<DirectionalLightComponent>(entity, out);
		TrySerializeEditor<PointLightComponent>(entity, out);

		//Meshes
		TrySerializeEditor<MeshComponent>(entity, out);
		TrySerializeEditor<SpriteRendererComponent>(entity, out);

		//Physics
		TrySerializeEditor<Rigidbody2DComponent>(entity, out);
		TrySerializeEditor<BoxCollider2DComponent>(entity, out);
		TrySerializeEditor<CircleCollider2DComponent>(entity, out);

		out << YAML::EndMap;
	}

	bool WorldSerializer::SerializeRuntime(const std::string& file)
	{
		return false;
	}
}
