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
	VarFieldType ScriptUtils::MonoTypeToFieldType(MonoType* type)
	{
		int t = mono_type_get_type(type);

		HZR_CORE_INFO(t);
		switch (t)
		{
		case MONO_TYPE_R4:			return VarFieldType::Float;
		case MONO_TYPE_I4:			return VarFieldType::Int;
		case MONO_TYPE_U4:			return VarFieldType::UnsignedInt;
		case MONO_TYPE_STRING:		return VarFieldType::String;
		case MONO_TYPE_CLASS:
		{
			char* name = mono_type_get_name(type);
			if (strcmp(name, "Hazard.Vector2") == 0) return VarFieldType::Vec2;
			if (strcmp(name, "Hazard.Vector3") == 0) return VarFieldType::Vec3;
			if (strcmp(name, "Hazard.Vector4") == 0) return VarFieldType::Vec4;
		}
		}
		return VarFieldType::None;
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