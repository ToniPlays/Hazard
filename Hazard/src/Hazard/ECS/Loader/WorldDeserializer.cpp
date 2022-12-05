
#include <hzrpch.h>
#include "WorldDeserializer.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard
{
	Ref<World> WorldDeserializer::DeserializeEditor(const std::filesystem::path& file, uint32_t flags)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;

		m_CanAsync = flags & AssetManagerFlags_CanAsync;
		YAML::Node root = YAML::LoadFile(file.string());

        Ref<World> world = Ref<World>::Create(file);

		//Set scene name
		if (!root["World"]) 
			return world;

		world->SetName(root["World"].as<std::string>());

		//Loop entities
		auto entities = root["Entities"];
		if (entities) {

			size_t index = 0;
			for (size_t i = entities.size(); i > 0; --i) 
			{
				auto node = entities[i - 1];
				UID uid = node["Entity"].as<uint64_t>();
				Entity entity = world->CreateEntity(uid, "");

				//Deserialize components
				TryDeserializeComponent<TagComponent>("TagComponent", entity, node);
				TryDeserializeComponent<TransformComponent>("TransformComponent", entity, node);
				TryDeserializeComponent<CameraComponent>("CameraComponent", entity, node);

				TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);

				TryDeserializeComponent<SkyLightComponent>("SkyLightComponent", entity, node);
				TryDeserializeComponent<DirectionalLightComponent>("DirectionalLightComponent", entity, node);
				TryDeserializeComponent<PointLightComponent>("PointLightComponent", entity, node);

				TryDeserializeComponent<MeshComponent>("MeshComponent", entity, node);
				TryDeserializeComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, node);

				TryDeserializeComponent<Rigidbody2DComponent>("Rigidbody2DComponent", entity, node);
				TryDeserializeComponent<BoxCollider2DComponent>("BoxCollider2DComponent", entity, node);
				TryDeserializeComponent<CircleCollider2DComponent>("CircleCollider2DComponent", entity, node);

				index++;

				if (m_Handler) 
					m_Handler(entity, index, entities.size());
			}
		}
		HZR_CORE_INFO("World loaded in {} ms", timer.ElapsedMillis());
		return world;
	}
	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node) 
	{
		if (!node[key]) return;

		Deserialize<T>(entity, node[key]);
	}
}
