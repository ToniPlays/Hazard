#pragma once

#include "Core.h"
#include "UtilityCore.h"
#include "ApplicationEvent.h"
#include "Core/Events/Events.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard 
{
	class Application;

	class HazardLoop 
	{
		friend class Application;
        
	public:
        HazardLoop();
		HazardLoop(Application* app);
		~HazardLoop();
        
        void SetApplication(Application* app);

		void Start();
		void Close();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);
		bool ShouldClose() { return m_ShouldClose; }
		uint64_t GetRenderedFrameCount() const { return m_FrameCount; }
		
		void Run();

		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }
        static ModuleHandler* GetModuleHandler() { return s_Instance->m_ModuleHandler.get(); }

	private:
		inline static HazardLoop* s_Instance;

	private:
		Application* m_Application = nullptr;
		Scope<ModuleHandler> m_ModuleHandler;

		uint64_t m_FrameCount = 0;
		bool m_ShouldClose = false;
	};
}
