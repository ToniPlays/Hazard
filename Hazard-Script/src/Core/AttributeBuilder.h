#pragma once

#include "Mono/Core/Mono.h"
#include "Attribute.h"

#include <unordered_map>
#include <functional>

namespace HazardScript
{
#ifdef HZR_INCLUDE_MONO
	using AttributeConstruct = std::function<Attribute* (MonoObject*)>;
#else
    using AttributeConstruct = std::function<Attribute*()>;
#endif

	class AttributeBuilder 
	{
	public:
#ifdef HZR_INCLUDE_MONO
		static Ref<Attribute> Create(const std::string className, MonoObject* obj);
#endif
		static void AddAttribute(const std::string& name, AttributeConstruct callback) 
		{
			s_Attribs[name] = callback;
		}
		static void Clear() 
		{
			s_Attribs.clear();
		}
	private:
		inline static std::unordered_map<std::string, AttributeConstruct> s_Attribs;
	};
}
