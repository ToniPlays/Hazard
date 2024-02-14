#pragma once

#include "UtilityCore.h"
#include "Event.h"
#include "UID.h"

namespace Hazard::ImUI
{
	class GUIRenderable 
	{
	public:
		GUIRenderable() = default;

		virtual void Update() {};
		virtual void Render() = 0;
		virtual bool OnEvent(Event& e) = 0;

		uint64_t GetPanelID() const { return m_PanelID; }

	private:
		UID m_PanelID = UID();
	};
}