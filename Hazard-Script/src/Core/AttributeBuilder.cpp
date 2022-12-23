
#include "AttributeBuilder.h"
#include "Mono/Core/Mono.h"

namespace HazardScript
{
#ifdef HZR_INCLUDE_MONO
	Ref<Attribute> AttributeBuilder::Create(const std::string className, MonoObject* obj)
	{
		for (auto& [name, cb] : s_Attribs) 
		{
			if (name == className) 
				return cb(obj);
		}
		return Ref<Attribute>();
	}
#endif
}
