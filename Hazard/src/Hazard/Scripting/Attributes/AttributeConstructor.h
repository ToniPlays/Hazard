#pragma once

#include "HazardScript.h"
#include "AllAttributes.h"

namespace Hazard
{
	class AttributeConstructor {
	public:
		AttributeConstructor() = default;
		~AttributeConstructor() = default;

		static void Init()
        {
			using namespace HazardScript;
#ifdef HZR_INCLUDE_MONO2
			AttributeBuilder::Clear();
			AttributeBuilder::AddAttribute("Author", [&](MonoObject* obj) {
				AuthorAttribute* attrib = hnew AuthorAttribute();
				attrib->Author = MonoFieldUtils::GetFieldValue<std::string>(obj, "Name");
				attrib->Version = MonoFieldUtils::GetFieldValue<double>(obj, "Version");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Todo", [&](MonoObject* obj) {
				TodoAttribute* attrib = hnew TodoAttribute();
				attrib->Detail = MonoFieldUtils::GetFieldValue<std::string>(obj, "Detail");
				attrib->Progress = (Progress)MonoFieldUtils::GetFieldValue<uint32_t>(obj, "Progress");
				return attrib;
				});
			AttributeBuilder::AddAttribute("MenuBarItem", [&](MonoObject* obj) {
				MenuBarItemAttribute* attrib = hnew MenuBarItemAttribute();
				attrib->Path = MonoFieldUtils::GetFieldValue<std::string>(obj, "Path");
				return attrib;
				});
			AttributeBuilder::AddAttribute("HideInProperties", [&](MonoObject* obj) {
				return hnew HideInPropertiesAttribute();
				});
			AttributeBuilder::AddAttribute("ShowInProperties", [&](MonoObject* obj) {
				return hnew ShowInPropertiesAttribute();
				});
			AttributeBuilder::AddAttribute("SerializeField", [&](MonoObject* obj) {
				return hnew SerializeFieldAttribute();
				});
			AttributeBuilder::AddAttribute("Slider", [&](MonoObject* obj) {
				SliderAttribute* attrib = hnew SliderAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<float>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Range", [&](MonoObject* obj) {
				RangeAttribute* attrib = hnew RangeAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<float>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("TextArea", [&](MonoObject* obj) {
				TextAreaAttribute* attrib = hnew TextAreaAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<int>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<int>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Header", [&](MonoObject* obj) {
				HeaderAttribute* attrib = hnew HeaderAttribute();
				attrib->Header = MonoFieldUtils::GetFieldValue<std::string>(obj, "Text");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Tooltip", [&](MonoObject* obj) {
				TooltipAttribute* attrib = hnew TooltipAttribute();
				attrib->Tooltip = MonoFieldUtils::GetFieldValue<std::string>(obj, "Text");
				return attrib;
				});
			AttributeBuilder::AddAttribute("VisualFunc", [&](MonoObject* obj) {
				return hnew VisualFuncAttribute();
				});
			AttributeBuilder::AddAttribute("ExposeFunc", [&](MonoObject* obj) {
				return hnew ExposeFuncAttribute();
				});
#endif
		}
	};
}
