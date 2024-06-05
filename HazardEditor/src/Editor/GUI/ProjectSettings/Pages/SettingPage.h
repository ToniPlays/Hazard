#pragma once

#include <iostream>
#include "Event.h"

class SettingPage
{
	public:
		SettingPage() = default;
		~SettingPage() = default;

		virtual const char* GetPageTitle() const = 0;
		virtual const char* GetPageDescription() const = 0;

		virtual void OnOpen() = 0;
		virtual void OnClose() = 0;
		virtual void RenderPage() = 0;
		virtual bool OnEvent(Event& e) { return false; };
};