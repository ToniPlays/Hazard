#pragma once
#include "Core/Core.h"
#include "core/Layers/Layer.h"

namespace Hazard {
	class HAZARD_API EditorGUI : public Layer {
	public:
		EditorGUI();
		~EditorGUI();
		void OnAttach();
		void OnImGuiRender();
	};
}