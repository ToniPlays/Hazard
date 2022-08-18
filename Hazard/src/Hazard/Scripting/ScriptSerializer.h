#pragma once

#include "Utility/YamlUtils.h"
#include "HazardScript.h"

namespace Hazard {
	class ScriptSerializer {
	public:
		static void SerializeFieldEditor(YAML::Emitter& out, HazardScript::FieldMetadata* field, Ref<HazardScript::ScriptObject> object);
		static void SerializeFieldValue(YAML::Emitter& out, const std::string& name, const std::string& key, const HazardScript::NativeType& type, Ref<HazardScript::ScriptObject> object);
	};
}