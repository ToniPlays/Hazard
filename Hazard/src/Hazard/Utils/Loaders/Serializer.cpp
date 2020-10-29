#pragma once
#include <hzrpch.h>
#include "Serializer.h"


namespace Hazard {

	bool Serializer::SerializeScene(const std::string& file, Scene& scene) {

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "HazardScene" << YAML::Value << scene.GetName();

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // Entities
		for (Entity* entity : scene.GetChildEntities()) 
			SerializeEntity(out, entity);
		out << YAML::EndSeq; // Entities

		out << YAML::EndMap;
		std::ofstream fout(file);
		fout << out.c_str();

		return true;
	}

	void Serializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		out << YAML::BeginMap; // Entity map
		out << YAML::Key << "Entity" << YAML::Value << "54389453978034207";
		out << YAML::Key << "Name" << YAML::Value << entity->name;

		out << YAML::Key << "Components" << YAML::Value << YAML::BeginMap; // Components
		for (Component* componet : entity->GetComponents()) 
			componet->SerializeComponent(out);

		out << YAML::EndMap; // Components
		out << YAML::EndMap; // Entity map
	}

	void Serializer::Serialize(YAML::Emitter& out, Vector2<float> value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
	}

	void Serializer::Serialize(YAML::Emitter& out, Vector3<float> value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
	}
	void Serializer::Serialize(YAML::Emitter& out, Color value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.r << value.g << value.b << value.a << YAML::EndSeq;
	}
}