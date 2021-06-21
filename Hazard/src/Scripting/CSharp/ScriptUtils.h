#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Scripting/PublicField.h"
#include "CSharpField.h"
#include "Mono/Mono.h"

extern "C"
{
	typedef struct _MonoString MonoString;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoType MonoType;
}

namespace Hazard::Scripting::CSharp {

	class ScriptUtils {
	public:

		static std::byte* AllocateBuffer(FieldType type);
		static uint32_t GetFieldSize(FieldType type);

		static void GetNames(const std::string& module, std::string& nameSpace, std::string& className);
		static std::string MonoStringToString(MonoString* string);
		static MonoString* StringToMonoString(const char* string);
		static std::string MonoObjectToChar(MonoObject* object);
		static FieldType GetFieldType(MonoClassField* field);
		static std::vector<PublicField*> MonoFieldsToFields(const std::vector<MonoClassField*>& fields);
	};
}