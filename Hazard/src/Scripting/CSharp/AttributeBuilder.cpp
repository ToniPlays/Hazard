#include "hzrpch.h"
#include "AttributeBuilder.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

namespace Hazard::Scripting::CSharp
{


	Attribute* AttributeBuilder::Create(const std::string className, MonoObject* obj)
	{
		if (className == "Author")
		{
			AuthorAttribute* attrib = new AuthorAttribute();
			attrib->Author = Mono::GetFieldValue<std::string>(obj, "Name");
			attrib->Version = Mono::GetFieldValue<double>(obj, "Version");
			return attrib;
		}
		else if (className == "Todo") 
		{
			TodoAttribute* attrib = new TodoAttribute();
			attrib->Detail = Mono::GetFieldValue<std::string>(obj, "Detail");
			attrib->Progress = (Progress)Mono::GetFieldValue<uint32_t>(obj, "Progress");
			return attrib;
		}
		else if (className == "HideInProperties") 
		{
			return new HideInPropertiesAttribute();
		}
		else if (className == "ShowInProperties")
		{
			return new ShowInPropertiesAttribute();
		}
		else if (className == "SerializeField")
		{
			return new SerializeFieldAttribute();
		}
		else if (className == "Slider")
		{
			SliderAttribute* attrib = new SliderAttribute();
			attrib->Min = Mono::GetFieldValue<float>(obj, "Min");
			attrib->Max = Mono::GetFieldValue<float>(obj, "Max");
			return attrib;
		}
		else if (className == "Range")
		{
			return new RangeAttribute();
		}
		else if (className == "Header")
		{
			return new HeaderAttribute();
		}
		else if (className == "Tooltip")
		{
			return new TooltipAttribute();
		}
		else if (className == "TextArea")
		{
			return new TextAreaAttribute();
		}
		else if (className == "VisualFunc")
		{
			return new VisualFuncAttribute();
		}
		else if (className == "ExposeFunc")
		{
			return new ExposeFuncAttribute();
		}

		HZR_CORE_ASSERT(false, "Unkonw attribute {0}", className);
		return new Attribute();
	}
}