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
				attrib->Author = Mono::GetFieldValue<std::string>(obj, "Name");
				attrib->Version = Mono::GetFieldValue<double>(obj, "Version");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Todo", [&](MonoObject* obj) {
				TodoAttribute* attrib = new TodoAttribute();
				attrib->Detail = Mono::GetFieldValue<std::string>(obj, "Detail");
				attrib->Progress = (Progress)Mono::GetFieldValue<uint32_t>(obj, "Progress");
				return attrib;
				});
			AttributeBuilder::AddAttribute("MenuBarItem", [&](MonoObject* obj) {
				MenuBarItemAttribute* attrib = new MenuBarItemAttribute();
				attrib->Path = Mono::GetFieldValue<std::string>(obj, "Path");
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
				attrib->Min = Mono::GetFieldValue<float>(obj, "Min");
				attrib->Max = Mono::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Range", [&](MonoObject* obj) {
				RangeAttribute* attrib = new RangeAttribute();
				attrib->Min = Mono::GetFieldValue<float>(obj, "Min");
				attrib->Max = Mono::GetFieldValue<float>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("TextArea", [&](MonoObject* obj) {
				TextAreaAttribute* attrib = new TextAreaAttribute();
				attrib->Min = Mono::GetFieldValue<int>(obj, "Min");
				attrib->Max = Mono::GetFieldValue<int>(obj, "Max");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Header", [&](MonoObject* obj) {
				HeaderAttribute* attrib = new HeaderAttribute();
				attrib->Header = Mono::GetFieldValue<std::string>(obj, "Text");
				return attrib;
				});
			AttributeBuilder::AddAttribute("Tooltip", [&](MonoObject* obj) {
				TooltipAttribute* attrib = new TooltipAttribute();
				attrib->Tooltip = Mono::GetFieldValue<std::string>(obj, "Text");
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