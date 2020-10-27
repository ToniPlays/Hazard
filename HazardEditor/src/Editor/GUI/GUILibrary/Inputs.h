#pragma once

class Inputs {
public:
	static void ResettableDragButton(const char* label, float& value, float resetValue, ImVec2 size, uint32_t buttonFont = 0, uint32_t dragFont = 0);
	static void InputField(std::string& text);
	static Color ColorPicker(const char* label, bool &open, Color color);
	static Color ColorPicker(std::string& label, bool &open, Color color);
	static Color LabelledColorPicker(const char* label, bool &open, Color color);
	static Color LabelledColorPicker(std::string& label, bool &open, Color color);
	static bool ConfirmDialog(const char* label, const char* description);
	static bool ConfirmDialog(std::string& label, std::string& description);

	static bool Combo(const char* label, std::vector<std::string> values, std::string& selected);
	static bool Combo(std::string label, std::vector<std::string> values, std::string& selected);

	static void BeginColumnRow(const char* label, uint8_t columns);
	static void BeginColumnRow(std::string label, uint8_t columns);

	static void EndColumnRow(uint8_t columns = 1);
	static void MaxWidth();

};