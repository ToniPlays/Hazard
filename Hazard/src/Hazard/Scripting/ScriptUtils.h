#pragma once

namespace Hazard::Scripting {
	
	enum class VarFieldType {
		None = 0, Float, Int, UnsignedInt, String, Vec2, Vec3, Vec4
	};


	class ScriptUtils {
	public:
		static void InitMono();
		static const char* VarFieldToString(VarFieldType type);
		static void GetNames(const std::string& module, std::string& nameSpace, std::string& className);
	};
}