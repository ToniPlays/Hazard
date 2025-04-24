#include <hzrpch.h>
#include "TextEditPanel.h"

#include "LangDefs/LanguageDefinitions.h"


namespace UI
{
	TextEditPanel::TextEditPanel() : ImUI::Panel("Text edit")
	{
		m_IsOpen = false;
	}

	bool TextEditPanel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchUntilHandled<KeyPressedEvent>(BIND_EVENT(TextEditPanel::OnKeyPressed));
		dispatcher.DispatchUntilHandled<MouseScrolledEvent>(BIND_EVENT(TextEditPanel::OnMouseScrolled));
		return dispatcher.DidHandle();
	}

	void TextEditPanel::OpenFile(const std::filesystem::path& path)
	{
		std::string source = File::ReadFile(path);
		m_TextEditor.SetText(source);
		m_Path = path;

		SetLanguageDefinition(File::GetFileExtension(path));

		m_Watcher = File::Watch(m_Path, [this](FileEvent e) {

			if (e == FileEvent::Modified && !m_Focused)
			{
				Application::Get().SubmitMainThread([this]() {
					m_TextEditor.SetText(File::ReadFile(m_Path));
					});
			}
			});
		BringToFront();
	}

	bool TextEditPanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if (!IsFocused()) return false;

		switch (e.GetKeyCode())
		{
			case Key::Backspace:
				m_TextEditor.Backspace();
				return true;
			case Key::Enter:
			{
				std::string line = m_TextEditor.GetCurrentLineText();
				uint64_t tabs = line.find_first_not_of("\t");
				m_TextEditor.AppendText("\n");

				if (tabs == std::string::npos)
					tabs = line.length();

				for (uint32_t i = 0; i < tabs; i++)
					m_TextEditor.AppendText("\t");

				return true;
			}
			case Key::Tab:
				m_TextEditor.AppendText("\t");
				return true;
			case Key::Delete:
				m_TextEditor.Delete();
				return true;
			case Key::Home:
				m_TextEditor.MoveHome();
				return true;
			case Key::End:
				m_TextEditor.MoveEnd();
				return true;
			case Key::PageUp:
				m_TextEditor.MoveTop();
				return true;
			case Key::PageDown:
				m_TextEditor.MoveBottom();
				return true;
			case Key::Left:
				m_TextEditor.MoveLeft();
				return true;
			case Key::Right:
				m_TextEditor.MoveRight();
				return true;
			case Key::Up:
				m_TextEditor.MoveUp();
				return true;
			case Key::Down:
				m_TextEditor.MoveDown();
				return true;
			case Key::S:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					Save();
					return true;
				}
				return false;
			}
			case Key::A:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.SelectAll();
					return true;
				}
				return false;
			}
			case Key::C:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.Copy();
					return true;
				}
				return false;
			}
			case Key::V:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.Paste();
					return true;
				}
				return false;
			}
			case Key::X:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.Cut();
					return true;
				}
				return false;
			}
			case Key::Z:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.Undo();
					return true;
				}
				return false;
			}
			case Key::Y:
			{
				if (Input::IsKeyDown(Key::LeftControl))
				{
					m_TextEditor.Redo();
					return true;
				}
				return false;
			}
		}
	}

	bool TextEditPanel::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (!Input::IsKeyDown(Key::LeftControl)) return false;

		m_TextSize += Math::Clamp<float>(e.GetYOffset(), 8.0f, 64.0f);
		m_TextEditor.SetUIFontSize(m_TextSize);
		return true;
	}

	void TextEditPanel::Save()
	{
		if (!File::Exists(m_Path))
		{
			m_Path = File::SaveFile({});
			if (m_Path.empty())
				return;
		}

		File::WriteFile(m_Path, m_TextEditor.GetText());

		AssetManager::Reload(AssetManager::AssetHandleFromFile(m_Path));
	}

	void TextEditPanel::SetLanguageDefinition(const std::string& extension)
	{
		Hooks<std::string, void()> hooks;

		hooks.AddHook(".cpp", [this]() {
			m_TextEditor.SetLanguageDefinition(LanguageDefinitions::CPlusPlus());
			});
		hooks.AddHook(".h", [this]() {
			m_TextEditor.SetLanguageDefinition(LanguageDefinitions::CPlusPlus());
			});

		hooks.AddHook(".glsl", [this]() {
			m_TextEditor.SetLanguageDefinition(LanguageDefinitions::GLSL());
			});
		hooks.AddHook(".shader", [this]() {
			m_TextEditor.SetLanguageDefinition(LanguageDefinitions::HazardShader());
			});

		hooks.Invoke(extension);
	}

	void TextEditPanel::OnPanelRender()
	{
		m_TextEditor.Render("Text");
	}
}
