
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
				TryDeserializeComponent<RelationshipComponent>("RelationshipComponent", entity, node);
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

	std::unordered_map<AssetHandle, uint32_t> WorldDeserializer::GetReferencedAssets()
	{
		std::unordered_map<AssetHandle, uint32_t> handles;
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
				TryGetReferencedAssets<CameraComponent>("CameraComponent", handles, node);
				TryGetReferencedAssets<ScriptComponent>("ScriptComponent", handles, node);

				TryGetReferencedAssets<SkyLightComponent>("SkyLightComponent", handles, node);
				TryGetReferencedAssets<DirectionalLightComponent>("DirectionalLightComponent", handles, node);
				TryGetReferencedAssets<PointLightComponent>("PointLightComponent", handles, node);

				TryGetReferencedAssets<MeshComponent>("MeshComponent", handles, node);
				TryGetReferencedAssets<SpriteRendererComponent>("SpriteRendererComponent", handles, node);

				TryGetReferencedAssets<Rigidbody2DComponent>("Rigidbody2DComponent", handles, node);
				TryGetReferencedAssets<BoxCollider2DComponent>("BoxCollider2DComponent", handles, node);
				TryGetReferencedAssets<CircleCollider2DComponent>("CircleCollider2DComponent", handles, node);

				m_Handler.Invoke(entities.size() - i, entities.size());
			}
		}

		return handles;
	}

	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, const YAML::Node& node)
	{
		if (node[key])
			Deserialize<T>(entity, node[key]);
	}
	template<typename T>
	void WorldDeserializer::TryGetReferencedAssets(const char* key, std::unordered_map<AssetHandle, uint32_t>& handles, const YAML::Node& node)
	{
		if (node[key])
			GetReferencedAssets<T>(handles, node[key]);
	}
}
