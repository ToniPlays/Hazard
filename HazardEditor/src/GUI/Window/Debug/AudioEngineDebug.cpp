
#include "AudioEngineDebug.h"

#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "GUI/EditorView.h"

using namespace WindowLayout;

namespace WindowElement
{
	AudioEngineDebug::AudioEngineDebug() : EditorWindow("Audio engine debugger")
	{
		SetActive(false);
	}
	void AudioEngineDebug::Init()
	{

	}
	void AudioEngineDebug::OnWindowRender()
	{
		HZR_PROFILE_FUNCTION();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

		Layout::Treenode("Audio files", flags, [&]() {});
	}
}
