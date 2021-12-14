#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard::Audio;

namespace WindowElement {
	class AudioEngineDebug : public EditorWindow {
	public:
		AudioEngineDebug();
		~AudioEngineDebug() = default;

		void Init() override;
		void OnWindowRender() override;

	private:
	};
}