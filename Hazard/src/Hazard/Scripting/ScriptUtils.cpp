#pragma once

#include "hzrpch.h"
#include "ScriptUtils.h"

#include "Hazard/File/File.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

namespace Hazard::Scripting {

	void ScriptUtils::InitMono() {
		mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
		mono_set_assemblies_path("C:/dev/Hazard//vendor/mono/lib");
	}
	const char* ScriptUtils::VarFieldToString(VarFieldType type)
	{
		switch (type)
		{
		case Hazard::Scripting::VarFieldType::Float:		return "Float";
		case Hazard::Scripting::VarFieldType::Int:			return "Int";
		case Hazard::Scripting::VarFieldType::UnsignedInt:	return "Unsigned int";
		case Hazard::Scripting::VarFieldType::String:		return "String";
		case Hazard::Scripting::VarFieldType::Vec2:			return "Vector 2";
		case Hazard::Scripting::VarFieldType::Vec3:			return "Vector 3";
		case Hazard::Scripting::VarFieldType::Vec4:			return "Vector 4";
		default:											return "Undefined";
		}
	}
	void ScriptUtils::GetNames(const std::string& module, std::string& nameSpace, std::string& className)
	{
		if (module.find('.') != std::string::npos) {
			nameSpace = module.substr(0, module.find_last_of("."));
			className = module.substr(module.find_last_of(".") + 1);
		}
		else className = module;
	}
}