#pragma once

#include "hzrpch.h"
#include "ScriptUtils.h"

#include "Hazard/File/File.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>


namespace Hazard::Scripting::CSharp {
	std::byte* ScriptUtils::AllocateBuffer(FieldType type)
	{
		uint32_t size = GetFieldSize(type);
		std::byte* buffer = new std::byte[size];
		memset(buffer, 0, size);
		return buffer;
	}
	uint32_t ScriptUtils::GetFieldSize(FieldType type)
	{
		switch (type)
		{
		case Hazard::Scripting::Float:	return 4;
		case Hazard::Scripting::Float2:	return 2 * 4;
		case Hazard::Scripting::Float3: return 3 * 4;
		case Hazard::Scripting::Float4:	return 4 * 4;
		case Hazard::Scripting::Int:	return 4;
		case Hazard::Scripting::UInt:	return 4;
		case Hazard::Scripting::String: return 16;
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
	std::string ScriptUtils::MonoStringToString(MonoString* string)
	{
		char* ptr = mono_string_to_utf8(string);
		std::string result(ptr);
		mono_free(ptr);
		return result;
	}
	MonoString* ScriptUtils::StringToMonoString(const char* string)
	{
		return mono_string_new(Mono::GetData().mono_domain, string);
	}
	char* ScriptUtils::MonoObjectToChar(MonoObject* object)
	{
		if (object == nullptr) {
			char* a = "NULL";
			return a;
		}
		MonoString* string = mono_object_to_string(object, nullptr);
		std::string b = MonoStringToString(string);
		return _strdup(b.c_str());
	}
	FieldType ScriptUtils::GetFieldType(MonoClassField* field)
	{
		MonoType* type = Mono::GetFieldType(field);

		switch (Mono::GetType(type))
		{
		case MONO_TYPE_R4:			return FieldType::Float;
		case MONO_TYPE_I4:			return FieldType::Int;
		case MONO_TYPE_U4:			return FieldType::UInt;
		case MONO_TYPE_STRING:		return FieldType::String;
		case MONO_TYPE_VALUETYPE:
		{

			char* name = Mono::GetTypeName(type);
			if (strcmp(name, "Hazard.Vector2") == 0) return FieldType::Float2;
			if (strcmp(name, "Hazard.Vector3") == 0) return FieldType::Float3;
			if (strcmp(name, "Hazard.Vector4") == 0) return FieldType::Float4;
		}
		}
		return FieldType::None;
	}
	std::vector<PublicField*> ScriptUtils::MonoFieldsToFields(std::vector<MonoClassField*> fields)
	{
		std::vector<PublicField*> result;

		for (MonoClassField* mono : fields) {
		
			CSharpField* field = new CSharpField(ScriptUtils::GetFieldType(mono));
			result.emplace_back(field);
		}

		return result;
	}
}