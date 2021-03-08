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
		case VarFieldType::Float:		return "Float";
		case VarFieldType::Int:			return "Int";
		case VarFieldType::UnsignedInt:	return "Unsigned int";
		case VarFieldType::String:		return "String";
		case VarFieldType::Vec2:		return "Vector 2";
		case VarFieldType::Vec3:		return "Vector 3";
		case VarFieldType::Vec4:		return "Vector 4";
		default:						return "Undefined";
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
	uint32_t ScriptUtils::GetFieldSize(VarFieldType type)
	{
		switch (type)
		{
		case VarFieldType::Float:		return 4;
		case VarFieldType::Int:			return 4;
		case VarFieldType::UnsignedInt:	return 4;
		//case VarFieldType::String:		return 16;
		case VarFieldType::Vec2:		return 4 * 2;
		case VarFieldType::Vec3:		return 4 * 3;
		case VarFieldType::Vec4:		return 4 * 4;
		default:						return 0;
		}
	}
	std::string ScriptUtils::MonoStringToString(MonoString* string)
	{
		char* ptr = mono_string_to_utf8(string);
		std::string result(ptr);
		mono_free(ptr);
		return result;
	}
	char* ScriptUtils::MonoObjectToChar(MonoObject* object)
	{
		if (object == nullptr)
			return (char*)'Null';
		else {
			MonoString* string = mono_object_to_string(object, nullptr);
			return _strdup(MonoStringToString(string).c_str());
		}
	}
}