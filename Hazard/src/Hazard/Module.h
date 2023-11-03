#pragma once
#include "Hazard/Core/Core.h"
#include "Event.h"

namespace Hazard {

	class Module {
	public:
		Module(const char* name) : m_Name(name) {}
		virtual ~Module() {}

		virtual void PreInit() {}
		virtual void Init() {}
		virtual void PreUpdate() {}
		virtual void Update() {}
		virtual void PostUpdate() {}
		virtual void PreRender() {}
		virtual void Render() {}
		virtual void PostRender() {}
		virtual void Close() {}
		virtual bool OnEvent(Event& e) { return false; }
		
		std::string GetName() { return m_Name; };
		bool IsActive() { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }

	private:
		std::string m_Name;
		bool m_IsActive = true;
	};
}