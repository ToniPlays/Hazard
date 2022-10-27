#pragma once

#include "Utility/YamlUtils.h"
#include "HazardScript.h"

namespace Hazard 
{
	class ScriptSerializer 
	{
	public:
		static void SerializeFieldEditor(Ref<HazardScript::ScriptObject> object, YAML::Emitter& out, Ref<HazardScript::FieldMetadata> field);
		static void SerializeFieldValue(YAML::Emitter& out, const std::string& name, const std::string& key, const HazardScript::NativeType& type, HazardScript::ScriptObject* object);

		static void DeserializeFieldEditor(Ref<HazardScript::ScriptObject> object, const std::string& name, YAML::Node& valueNode);
		static void DeserializeFieldValue(const std::string& name, Ref<HazardScript::ScriptObject> object, YAML::Node& valueNode, const HazardScript::NativeType& type);
	};
}