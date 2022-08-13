#pragma once

#include "Mono/Core/Mono.h"
#include "Attribute.h"

#include <unordered_map>
#include <functional>

namespace HazardScript
{
	using AttributeConstruct = std::function<Attribute* (MonoObject*)>;
	class AttributeBuilder 
	{
	public:
		static Attribute* Create(const std::string className, MonoObject* obj);
		static void AddAttribute(const std::string& name, AttributeConstruct callback) {
			s_Attribs[name] = callback;
		}
		static void Clear() {
			s_Attribs.clear();
		}
	private:
		inline static std::unordered_map<std::string, AttributeConstruct> s_Attribs;
	};
}