#pragma once

#include <hzreditor.h>
#include "Console.h"

#include "GUI/Library/Style.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/EditorView.h"

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

using namespace WindowLayout;
using namespace Hazard::Scripting;

namespace WindowElement {

	static void PrintToConsole(Severity severity, std::string message) 
	{
		Console& instance = EditorView::GetInstance().GetRenderable<Console>();
		instance.Print(severity, message);
	}

	Console::Console() : EditorWindow(ICON_FK_COMPASS " Console")
	{

	}
	Console::~Console()
	{

	}
	static void Print() {
		
	}
	void Console::Init()
	{
		void(*callback)(Severity, std::string) = PrintToConsole;
		ScriptCommand::SetDebugCallback(callback);

	}
	void Console::OnWindowRender()
	{
		ImGuiStyle& style = Style::Style::GetStyle();
		if (Input::Button("Clear")) {
			ClearLog();
		}
		Layout::SameLine(0, 5);
		Input::DynamicToggleButton(ICON_FK_TIMES, ICON_FK_CHECK, Color::FromHex("#B31A1A"), Color::FromHex("#338033"), scrollLockEnable);

		Color enableColor = Color::FromHex("#181818");
		Color disableColor = Color::FromHex("#803333");

		Layout::SameLine(0, ImGui::GetContentRegionAvailWidth() - 200);
		Input::ToggleButtonColorChange(ICON_FK_PAPERCLIP,				disableColor, enableColor, Color::FromHex("#FFFFFF"), traceEnable);
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_INFO_CIRCLE,				disableColor, enableColor, Color::FromHex("#1AE61A"), infoEnable);
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_BUG,						disableColor, enableColor, Color::FromHex("#0080ff"), debugEnable);
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_EXCLAMATION_TRIANGLE,	disableColor, enableColor, Color::FromHex("#FFE600"), warningEnable);
		Layout::SameLine(0, 5);
		Input::ToggleButtonColorChange(ICON_FK_EXCLAMATION_CIRCLE,		disableColor, enableColor, Color::FromHex("#FF331A"), errorEnable);

		ImGui::BeginChild(ICON_FK_LIST" Console", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (auto& [severity, message] : messages) {
			if (!LogEnabled(severity)) continue;
			
			switch (severity)
			{
				case Severity::Trace:
				{
					Layout::TextColored(ICON_FK_PAPERCLIP " [Trace]: ", Color::FromHex("#FFFFFF"));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Info:
				{
					Layout::TextColored(ICON_FK_INFO_CIRCLE " [Info]: ", Color::FromHex("#1AE61A"));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Debug:
				{
					Layout::TextColored(ICON_FK_BUG " [Log]: ", Color::FromHex("#0080ff"));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Warning: 
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_TRIANGLE " [Warning]: ", Color::FromHex("#FFE600"));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Error:
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_CIRCLE " [Error]: ", Color::FromHex("#FF331A"));
					Layout::SameLine(90);
					Layout::Text(message.c_str());
					break;
				}
				case Severity::Critical:
				{
					Layout::TextColored(ICON_FK_EXCLAMATION_CIRCLE " [Critical]: ", Color::FromHex("#FF0000"));
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
		HZR_WARN(message);
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