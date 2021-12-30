#pragma once
#include <hzrpch.h>
#include "GUI/EditorWindow.h"

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