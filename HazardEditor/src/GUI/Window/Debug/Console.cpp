#pragma once

#include <hzreditor.h>
#include "Console.h"

#include "GUI/Library/Style.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/EditorView.h"

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

using namespace WindowLayout;
using namespace Hazard::Scripting;

namespace WindowElement {

	static void PrintScriptToConsole(Severity severity, std::string message) 
	{
		Console& instance = *EditorView::GetInstance().GetRenderable<Console>();
		instance.Print(severity, message);
	}
	static void PrintDebugToConsole(Rendering::ErrorData& data)
	{
		Console& instance = *EditorView::GetInstance().GetRenderable<Console>();
		instance.Print((Severity)data.severity, data.info);
	}

	Console::Console() : EditorWindow(ICON_FK_COMPASS " Console") {}
	Console::~Console() {}
	void Console::Init()
	{
		void(*scriptCallback)(Severity, std::string) = PrintScriptToConsole;
		ScriptCommand::SetDebugCallback(scriptCallback);

		Rendering::ErrorCallback renderCallback = PrintDebugToConsole;

		Rendering::RenderContextCommand::SetDebugCallback(renderCallback);
	}
	void Console::OnWindowRender()
	{
		if (Input::Button("Clear")) {
			ClearLog();
		}

		Layout::SameLine(0, 5);
		Input::DynamicToggleButton(ICON_FK_TIMES, ICON_FK_CHECK, Style::GetStyleColor(ColorType::Background), 
			Style::GetStyleColor(ColorType::Debug), scrollLockEnable, { 25, 0 });
		Layout::Tooltip("Toggle scrolling");

		Color enableColor = Style::GetStyleColor(ColorType::Background);
		Color disableColor = Color::FromHex("#803333");

		Layout::SameLine(0, ImGui::GetContentRegionAvailWidth() - 200);
		Input::ToggleButtonColorChange(ICON_FK_PAPERCLIP,				disableColor, enableColor, Style::GetStyleColor(ColorType::Trace), traceEnable);
		Layout::Tooltip("Trace");

		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_INFO_CIRCLE,				disableColor, enableColor, Style::GetStyleColor(ColorType::Info), infoEnable);
		Layout::Tooltip("Info");

		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_BUG,						disableColor, enableColor, Style::GetStyleColor(ColorType::Debug), debugEnable);
		Layout::Tooltip("Debug");

		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_EXCLAMATION_TRIANGLE,	disableColor, enableColor, Style::GetStyleColor(ColorType::Warning), warningEnable);
		Layout::Tooltip("Warning");

		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_EXCLAMATION_CIRCLE,		disableColor, enableColor, Style::GetStyleColor(ColorType::Error), errorEnable);
		Layout::Tooltip("Error");

		ImGui::BeginChild(ICON_FK_LIST" Console", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (auto& [severity, message] : messages) {
			if (!LogEnabled(severity)) continue;
			
			switch (severity)
			{
				case Severity::Trace:
				{
					Layout::TextColored(ICON_FK_PAPERCLIP " [Trace]: ", Style::GetStyleColor(ColorType::Trace));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Info:
				{
					Layout::TextColored(ICON_FK_INFO_CIRCLE " [Info]: ", Style::GetStyleColor(ColorType::Info));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Debug:
				{
					Layout::TextColored(ICON_FK_BUG " [Log]: ", Style::GetStyleColor(ColorType::Debug));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Warning: 
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_TRIANGLE " [Warning]: ", Style::GetStyleColor(ColorType::Warning));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Error:
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_CIRCLE " [Error]: ", Style::GetStyleColor(ColorType::Error));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Critical:
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_CIRCLE " [Critical]: ", Style::GetStyleColor(ColorType::Critical));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
			}

		}
		if (scrollLockEnable)
			ImGui::SetScrollY(ImGui::GetScrollMaxY() + 100);
		Layout::NextLine(15);
		ImGui::EndChild();
	}

	void Console::Print(Severity severity, std::string message)
	{
		messages.emplace_back(std::pair<Severity, std::string>(severity, message));
	}

	bool Console::LogEnabled(Severity severity)
	{
		switch (severity)
		{

		case Severity::Trace:		return traceEnable;
		case Severity::Info:		return infoEnable;
		case Severity::Debug:		return debugEnable;
		case Severity::Warning:		return warningEnable;
		case Severity::Error:		return errorEnable;
		case Severity::Critical:	return true;
		}
	}
}