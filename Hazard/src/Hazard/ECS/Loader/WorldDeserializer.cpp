
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

			for (size_t i = entities.size(); i > 0; --i)
			{
				auto node = entities[i - 1];
				UID uid = node["Entity"].as<uint64_t>();
				Entity entity = world->GetEntityFromUID(uid);
				TryDeserializeComponent<RelationshipComponent>("RelationshipComponent", entity, node);
			}
		}

		return world;
	}

	AssetReferences WorldDeserializer::GetReferencedAssets()
	{
		AssetReferences references;
		YAML::Node root = YAML::Load(m_Source.c_str());

		if (!root["World"])
			return AssetReferences();

		auto entities = root["Entities"];
		if (entities)
		{
			for (size_t i = entities.size(); i > 0; --i)
			{
				auto node = entities[i - 1];
				std::string name;
				YamlUtils::Deserialize<std::string>(node["TagComponent"], "Tag", name, "");

				//Deserialize components
				TryGetReferencedAssets<CameraComponent>("CameraComponent", references, node, name);
				TryGetReferencedAssets<ScriptComponent>("ScriptComponent", references, node, name);

				TryGetReferencedAssets<SkyLightComponent>("SkyLightComponent", references, node, name);
				TryGetReferencedAssets<DirectionalLightComponent>("DirectionalLightComponent", references, node, name);
				TryGetReferencedAssets<PointLightComponent>("PointLightComponent", references, node, name);

				TryGetReferencedAssets<MeshComponent>("MeshComponent", references, node, name);
				TryGetReferencedAssets<SpriteRendererComponent>("SpriteRendererComponent", references, node, name);

				TryGetReferencedAssets<Rigidbody2DComponent>("Rigidbody2DComponent", references, node, name);
				TryGetReferencedAssets<BoxCollider2DComponent>("BoxCollider2DComponent", references, node, name);
				TryGetReferencedAssets<CircleCollider2DComponent>("CircleCollider2DComponent", references, node, name);

				m_Handler.Invoke(entities.size() - i, entities.size());
			}
		}

		return references;
	}

	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, const YAML::Node& node)
	{
		if (node[key])
			Deserialize<T>(entity, node[key]);
	}
	template<typename T>
	void WorldDeserializer::TryGetReferencedAssets(const char* key, AssetReferences& references, const YAML::Node& node, const std::string& entityName)
	{
		if (node[key])
			GetReferencedAssets<T>(references, node[key], entityName);
	}
}
