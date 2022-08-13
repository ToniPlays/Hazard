
#include "AttributeBuilder.h"
#include "Mono/Core/Mono.h"

namespace HazardScript
{
	Attribute* AttributeBuilder::Create(const std::string className, MonoObject* obj)
	{
		for (auto& [name, cb] : s_Attribs) {
			if (name == className) {
				return cb(obj);
			}
		}
		return new Attribute();
	}
}