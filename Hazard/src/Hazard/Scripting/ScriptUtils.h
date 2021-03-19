#pragma once

extern "C"
{
	typedef struct _MonoString MonoString;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoType MonoType;
}

namespace Hazard::Scripting {

	using BindMap = std::vector<std::pair<const char*, void*>>;
	
	enum class VarFieldType {
		None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
	};

	class ScriptUtils {
	public:
		static const char* VarFieldToString(VarFieldType type);
		static uint32_t GetFieldSize(VarFieldType type);
		static VarFieldType ScriptUtils::MonoTypeToFieldType(MonoType* type);

		static void GetNames(const std::string& module, std::string& nameSpace, std::string& className);
		static std::string MonoStringToString(MonoString* string);
		static MonoString* StringToMonoString(const char* string);
		static char* MonoObjectToChar(MonoObject* object);
	};
}