#pragma once

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

	}
	void AudioEngineDebug::Init()
	{

	}
	void AudioEngineDebug::OnWindowRender()
	{
		using namespace Hazard::Audio;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

		Layout::Treenode("Audio files", flags, [&]() {});
	}
}