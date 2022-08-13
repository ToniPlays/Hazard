
#include "AttributeBuilder.h"
#include "Attributes/AllAttributes.h"
#include "Mono/Core/Mono.h"

namespace HazardScript
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
		else if (className == "MenuBarItem")
		{
			MenuBarItemAttribute* attrib = new MenuBarItemAttribute();
			attrib->Path = Mono::GetFieldValue<std::string>(obj, "Path");
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
			RangeAttribute* attrib = new RangeAttribute();
			attrib->Min = Mono::GetFieldValue<float>(obj, "Min");
			attrib->Max = Mono::GetFieldValue<float>(obj, "Max");
			return attrib;
		}
		else if (className == "TextArea")
		{
			TextAreaAttribute* attrib = new TextAreaAttribute();
			attrib->Min = Mono::GetFieldValue<int>(obj, "Min");
			attrib->Max = Mono::GetFieldValue<int>(obj, "Max");
			return attrib;
		}
		else if (className == "Header")
		{
			HeaderAttribute* attrib = new HeaderAttribute();
			attrib->Header = Mono::GetFieldValue<std::string>(obj, "Text");
			return attrib;
		}
		else if (className == "Tooltip")
		{
			TooltipAttribute* attrib = new TooltipAttribute();
			attrib->Tooltip = Mono::GetFieldValue<std::string>(obj, "Text");
			return attrib;
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
		return new Attribute();
	}
}