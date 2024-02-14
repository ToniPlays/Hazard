
#include "Console.h"
#include "Core/MessageFlags.h"

#include "Hazard/ImGUI/UIElements/Table.h"


namespace UI
{
	Console::Console() : Panel("Console", false)
	{
		auto& window = Application::Get().GetModule<RenderContextManager>().GetWindow();
		auto& scriptEngine = Application::Get().GetModule<ScriptEngine>();

		window.AddDebugCallback([this](const RenderMessage& message) {
			uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
			AddMessage({ message.Description, message.StackTrace, messageFlags });
		});

		scriptEngine.AddDebugCallback([this](const HazardScript::ScriptMessage& message) {
			uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
			AddMessage({ message.Message, message.StackTrace, GetMessageFlagsFromSeverity(Severity::Error) });
		});

		JobSystem& system = Application::Get().GetJobSystem();
		system.Hook(JobSystem::Message, [this](Severity severity, const std::string& message) {
			Application::Get().SubmitMainThread([this, severity, message]() {
				AddMessage({ message, "", GetMessageFlagsFromSeverity(severity) });
			});
		});
	}

	bool Console::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Console::KeyPressed));
	}

	void Console::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();
		consoleSize.y -= 36.0f;

		DrawToolbar({ consoleSize.x, 32.0f });

		ImUI::Table<ConsoleMessage> table("Console", consoleSize);
		table.SetColumns({ "Type", "Timestamp", "Message" });
		table.RowHeight(24.0f);
		table.RowContent([&](uint32_t, const ConsoleMessage& message) {
			ImGui::PushID(message.Message.c_str());

			ImUI::Separator({ 4.0, 24.0f }, GetMessageColor(message.Flags));
			ImGui::SameLine();
			ImGui::Text("%s", GetMessageType(message));
			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("Time");

			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", message.Message.c_str());

			ImGui::PopID();
		});

		for (auto& message : m_Messages)
		{
			if (!(message.Flags & m_DisplayFlags) && m_DisplayFlags) continue;
			table.AddRow(message);
		}

		table.Render();

		static ConsoleMessage selectedMessage;

		if (table.DidSelect())
		{
			selectedMessage = table.SelectedValue();
			ImGui::OpenPopup(selectedMessage.Message.c_str());

			ImVec2 size = ImGui::GetMainViewport()->Size;
			ImGui::SetNextWindowSize({ size.x * 0.5f, size.y * 0.5f });
			ImGui::SetNextWindowPos({ size.x / 2.0f, size.y / 2.5f }, 0, { 0.5, 0.5 });
			m_DetailedPanelOpen = true;
		}

		ImUI::ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		ImUI::ScopedStyleVar framePadding(ImGuiStyleVar_FramePadding, ImVec2(4, 8));
		ImUI::ScopedStyleColor color(ImGuiCol_PopupBg, style.ChildBackgroundColor);

		if (ImGui::BeginPopupModal(selectedMessage.Message.c_str(), &m_DetailedPanelOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{

			ImGui::Text("%s", selectedMessage.Description.c_str());
			ImGui::EndPopup();
		}
	}

	bool Console::KeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Escape && m_DetailedPanelOpen)
		{
			m_DetailedPanelOpen = false;
			return true;
		}
		return false;
	}

	void Console::Clear(bool force)
	{
		std::vector<ConsoleMessage> messages;
		uint64_t size = m_Messages.size();
		for (uint64_t i = 0; i < size; i++)
		{
			ConsoleMessage& message = m_Messages[i];
			if (!(message.Flags & MessageFlags_Clearable || force))
			{
				messages.push_back(message);
			}
		}
		m_Messages = messages;
	}
	const char* Console::GetMessageType(const ConsoleMessage& message)
	{
		if (message.Flags & MessageFlags_None)		return "Log";
		if (message.Flags & MessageFlags_Debug)		return "Debug";
		if (message.Flags & MessageFlags_Info)		return "Info";
		if (message.Flags & MessageFlags_Warning)	return "Warning";
		if (message.Flags & MessageFlags_Error)		return "Error";
		if (message.Flags & MessageFlags_Fatal)		return "Fatal";
		return "Unknown";
	}

	void Console::DrawToolbar(const ImVec2& size)
	{
		using namespace ImUI;
		const ImUI::Style& style = StyleManager::GetCurrent();
		{
			ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::BeginChild("Toolbar", size);
			{
				ScopedColorStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
				if (ImGui::Button("Clear", { 75.0f, 32.0f }))
				{
					Clear();
				}
			}
			ImGui::SameLine(0, 5);
			if (ColoredButton("Clear on build", (m_ClearOnBuild ? style.Frame.FrameColor : style.Window.Header), style.Window.Text, { 100.0f, 32.0f }))
			{
				m_ClearOnBuild = !m_ClearOnBuild;
			}

			ImGui::SameLine(0, 5);
			if (ColoredButton("Clear on play", (m_ClearOnPlay ? style.Frame.FrameColor : style.Window.Header), style.Window.Text, { 100.0f, 32.0f }))
			{
				m_ClearOnPlay = !m_ClearOnPlay;
			}

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 154, 0);


			if (ColoredButton((const char*)ICON_FK_PAPERCLIP, (m_DisplayFlags & MessageFlags_Debug ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Debug), { 32.0f, 32.0f }))
			{
				m_DisplayFlags ^= MessageFlags_Debug;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_INFO_CIRCLE, (m_DisplayFlags & MessageFlags_Info ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Info), { 32.0f, 32.0f }))
			{
				m_DisplayFlags ^= MessageFlags_Info;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_EXCLAMATION_TRIANGLE, (m_DisplayFlags & MessageFlags_Warning ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Warning), { 32.0f, 32.0f }))
			{
				m_DisplayFlags ^= MessageFlags_Warning;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_EXCLAMATION_CIRCLE, (m_DisplayFlags & MessageFlags_Error ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Error), { 32.0f, 32.0f }))
			{
				m_DisplayFlags ^= MessageFlags_Error;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_BUG, (m_DisplayFlags & MessageFlags_Fatal ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Fatal), { 32.0f, 32.0f }))
			{
				m_DisplayFlags ^= MessageFlags_Fatal;
			}


		}
		ImGui::EndChild();
	}

	ImVec4 Console::GetMessageColor(const uint32_t& flag)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		if (flag & MessageFlags_None)		return style.Colors.AxisZ;
		if (flag & MessageFlags_Info)		return style.Colors.AxisZ;
		if (flag & MessageFlags_Debug)		return style.Colors.AxisY;
		if (flag & MessageFlags_Warning)	return style.Colors.Warning;
		if (flag & MessageFlags_Error)		return style.Colors.AxisX;
		if (flag & MessageFlags_Fatal)		return style.Colors.AxisX;

		return style.Window.Header;
	}
}
