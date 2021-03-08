#pragma once


extern "C"
{
	typedef struct _MonoString MonoString;
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting {
	
	enum class VarFieldType {
		None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
	};


	using BindMap = std::vector<std::pair<const char*, void*>>;

	class ScriptUtils {
	public:
		static void InitMono();
		static const char* VarFieldToString(VarFieldType type);
		static void GetNames(const std::string& module, std::string& nameSpace, std::string& className);
		static uint32_t GetFieldSize(VarFieldType type);
		static std::string MonoStringToString(MonoString* string);
		static char* MonoObjectToChar(MonoObject* object);
	};
}