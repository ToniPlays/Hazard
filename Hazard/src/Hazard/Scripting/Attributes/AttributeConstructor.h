#pragma once

#include "HazardScript.h"
#include "AllAttributes.h"

namespace Hazard
{
	class AttributeConstructor {
	public:
		AttributeConstructor() = default;
		~AttributeConstructor() = default;

		static void Init() {
			using namespace HazardScript;

			AttributeBuilder::Clear();
			AttributeBuilder::AddAttribute("Author", [&](MonoObject* obj) {
				AuthorAttribute* attrib = new AuthorAttribute();
				attrib->Author = MonoFieldUtils::GetFieldValue<std::string>(obj, "Name");
				attrib->Version = MonoFieldUtils::GetFieldValue<double>(obj, "Version");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Todo", [&](MonoObject* obj) {
				TodoAttribute* attrib = new TodoAttribute();
				attrib->Detail = MonoFieldUtils::GetFieldValue<std::string>(obj, "Detail");
				attrib->Progress = (Progress)MonoFieldUtils::GetFieldValue<uint32_t>(obj, "Progress");
				return attrib;
				});
			AttributeBuilder::AddAttribute("MenuBarItem", [&](MonoObject* obj) {
				MenuBarItemAttribute* attrib = new MenuBarItemAttribute();
				attrib->Path = MonoFieldUtils::GetFieldValue<std::string>(obj, "Path");
				return attrib;
				});
			AttributeBuilder::AddAttribute("HideInProperties", [&](MonoObject* obj) {
				return new HideInPropertiesAttribute();
				});
			AttributeBuilder::AddAttribute("ShowInProperties", [&](MonoObject* obj) {
				return new ShowInPropertiesAttribute();
				});
			AttributeBuilder::AddAttribute("SerializeField", [&](MonoObject* obj) {
				return new SerializeFieldAttribute();
				});
			AttributeBuilder::AddAttribute("Slider", [&](MonoObject* obj) {
				SliderAttribute* attrib = new SliderAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<float>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Range", [&](MonoObject* obj) {
				RangeAttribute* attrib = new RangeAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<float>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("TextArea", [&](MonoObject* obj) {
				TextAreaAttribute* attrib = new TextAreaAttribute();
				attrib->Min = MonoFieldUtils::GetFieldValue<int>(obj, "Min");
				attrib->Max = MonoFieldUtils::GetFieldValue<int>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Header", [&](MonoObject* obj) {
				HeaderAttribute* attrib = new HeaderAttribute();
				attrib->Header = MonoFieldUtils::GetFieldValue<std::string>(obj, "Text");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Tooltip", [&](MonoObject* obj) {
				TooltipAttribute* attrib = new TooltipAttribute();
				attrib->Tooltip = MonoFieldUtils::GetFieldValue<std::string>(obj, "Text");
				return attrib;
				});
			AttributeBuilder::AddAttribute("VisualFunc", [&](MonoObject* obj) {
				return new VisualFuncAttribute();
				});
			AttributeBuilder::AddAttribute("ExposeFunc", [&](MonoObject* obj) {
				return new ExposeFuncAttribute();
				});

		}
	};
}