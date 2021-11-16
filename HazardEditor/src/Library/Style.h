#pragma once

#include "Hazard.h"
#include "imgui.h"
#include "imgui_internal.h"

using namespace Hazard;

enum ColorType {
	Primary,
	Secondary,
	Info,
	Debug,
	Warning,
	Error,
	Critical,
	Trace,
	Background,
	Text
};

enum Theme {
	Dark,
	Classic
};

struct ThemeValues
{
	ImVec2 dockPadding;
	ImVec2 framePadding;
};

class ScopedStyle 
{
public:
	ScopedStyle(const ScopedStyle&) = delete;
	ScopedStyle operator=(const ScopedStyle&) = delete;
	template<typename T>
	ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
	~ScopedStyle() { ImGui::PopStyleVar(); }
};

class ScopedColour
{
public:
	ScopedColour(const ScopedColour&) = delete;
	ScopedColour operator=(const ScopedColour&) = delete;
	template<typename T>
	ScopedColour(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, colour); }
	~ScopedColour() { ImGui::PopStyleColor(); }
};

class ScopedColourStack
{
public:
    ScopedColourStack(const ScopedColourStack&) = delete;
    ScopedColourStack operator=(const ScopedColourStack&) = delete;

    template <typename ColourType, typename... OtherColours>
    ScopedColourStack(ImGuiCol firstColourID, ColourType firstColour, OtherColours&& ... otherColourPairs)
        : m_Count((sizeof... (otherColourPairs) / 2) + 1)
    {
        static_assert ((sizeof... (otherColourPairs) & 1u) == 0,
            "ScopedColourStack constructor expects a list of pairs of colour IDs and colours as its arguments");

        PushColour(firstColourID, firstColour, std::forward<OtherColours>(otherColourPairs)...);
    }

    ~ScopedColourStack() { ImGui::PopStyleColor(m_Count); }

private:
    int m_Count;

    template <typename ColourType, typename... OtherColours>
    void PushColour(ImGuiCol colourID, ColourType colour, OtherColours&& ... otherColourPairs)
    {
        if constexpr (sizeof... (otherColourPairs) == 0)
        {
            ImGui::PushStyleColor(colourID, colour);
        }
        else
        {
            ImGui::PushStyleColor(colourID, colour);
            PushColour(std::forward<OtherColours>(otherColourPairs)...);
        }
    }
};

class ScopedStyleStack
{
public:
    ScopedStyleStack(const ScopedStyleStack&) = delete;
    ScopedStyleStack operator=(const ScopedStyleStack&) = delete;

    template <typename ValueType, typename... OtherStylePairs>
    ScopedStyleStack(ImGuiStyleVar firstStyleVar, ValueType firstValue, OtherStylePairs&& ... otherStylePairs)
        : m_Count((sizeof... (otherStylePairs) / 2) + 1)
    {
        static_assert ((sizeof... (otherStylePairs) & 1u) == 0,
            "ScopedStyleStack constructor expects a list of pairs of colour IDs and colours as its arguments");

        PushStyle(firstStyleVar, firstValue, std::forward<OtherStylePairs>(otherStylePairs)...);
    }

    ~ScopedStyleStack() { ImGui::PopStyleVar(m_Count); }

private:
    int m_Count;

    template <typename ValueType, typename... OtherStylePairs>
    void PushStyle(ImGuiStyleVar styleVar, ValueType value, OtherStylePairs&& ... otherStylePairs)
    {
        if constexpr (sizeof... (otherStylePairs) == 0)
        {
            ImGui::PushStyleVar(styleVar, value);
        }
        else
        {
            ImGui::PushStyleVar(styleVar, value);
            PushStyle(std::forward<OtherStylePairs>(otherStylePairs)...);
        }
    }
};


class Style {
public:
	static void InitTheme(Theme theme);
	static void SetColor(ImGuiCol_ color, const Color& value);
	static void SetColor(ImGuiCol_ color, const ImVec4& value);
	static void SetButtonColors(const char* button, const char* hovered, const char* active);
	static void SetButtonColors(const Color& button, const Color& hovered, const Color& active);
	static void SelectFont(uint8_t index);
	static ImFont* AddFont(const char* file, float size);
	static ImVec4 ColorAsImVec4(const Color& color);
	static ImU32 ColorWithMultiplier(ImVec4 color, float multiplier);

	static ImGuiTreeNodeFlags GetTreeNodeDefaultFlags();
	static ThemeValues& GetStyle() { return s_Values; }
	static Color GetStyleColor(ColorType type);
	static ImU32 GetStyleColor32(ColorType type);
private:
	static void InitClassic();
	static void InitDarkTheme();
private:
	static Theme s_Theme;
	static ThemeValues s_Values;
};
