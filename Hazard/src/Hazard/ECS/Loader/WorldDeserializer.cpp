
#include <hzrpch.h>
#include "WorldDeserializer.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard
{
	Ref<World> WorldDeserializer::Deserialize()
	{
		YAML::Node root = YAML::Load(m_Source.c_str());

		if (!root["World"])
			return Ref<World>::Create("New world");

		Ref<World> world = Ref<World>::Create(root["World"].as<std::string>());

		//Loop entities
		auto entities = root["Entities"];
		if (entities)
		{
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

				m_Handler.Invoke(entities.size() - i, entities.size());
			}
		}
		return world;
	}

	std::vector<AssetHandle> WorldDeserializer::GetReferencedAssets()
	{
		std::vector<AssetHandle> handles;
		YAML::Node root = YAML::Load(m_Source.c_str());

		if (!root["World"])
			return handles;

		auto entities = root["Entities"];
		if (entities)
		{
			for (size_t i = entities.size(); i > 0; --i)
			{
				auto node = entities[i - 1];

				//Deserialize components
				GetReferencedAssets<CameraComponent>("CameraComponent", handles, node);
				GetReferencedAssets<ScriptComponent>("ScriptComponent", handles, node);

				GetReferencedAssets<SkyLightComponent>("SkyLightComponent", handles, node);
				GetReferencedAssets<DirectionalLightComponent>("DirectionalLightComponent", handles, node);
				GetReferencedAssets<PointLightComponent>("PointLightComponent", handles, node);

				GetReferencedAssets<MeshComponent>("MeshComponent", handles, node);
				GetReferencedAssets<SpriteRendererComponent>("SpriteRendererComponent", handles, node);

				GetReferencedAssets<Rigidbody2DComponent>("Rigidbody2DComponent", handles, node);
				GetReferencedAssets<BoxCollider2DComponent>("BoxCollider2DComponent", handles, node);
				GetReferencedAssets<CircleCollider2DComponent>("CircleCollider2DComponent", handles, node);

				m_Handler.Invoke(entities.size() - i, entities.size());
			}
		}

		return handles;
	}

	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node)
	{
		if (node[key])
			Deserialize<T>(entity, node[key]);
	}
}
