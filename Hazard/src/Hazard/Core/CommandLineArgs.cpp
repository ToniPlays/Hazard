#include "hzrpch.h"
#include "CommandLineArgs.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard 
{
	void CommandLineArgs::Init(int count, char* values[])
	{
		for (uint32_t i = 1; i < count; i += 2) 
		{
			const char* key = values[i];
			m_Arguments[key] = values[i + 1];
		}
	}
	template<typename T>
	static T CommandLineArgs::Get() {
		static_assert(false);
	}
	template<>
	static RenderAPI CommandLineArgs::Get() {
		return Rendering::RenderContext::StringToApi(m_Arguments["-RenderAPI"]);
	}
}
