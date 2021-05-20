#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard::Module {
	class Module {
	public:
		Module(const char* name) : m_Name(name) {}
		~Module() {}

		virtual void PreInit() {}
		virtual void Init() {}
		virtual void Update() {}
		virtual void Render() {}
		virtual void Close() {}
		
		std::string GetName() { return m_Name; };
		bool GetActive() { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
	private:
		std::string m_Name;
		bool m_IsActive = false;
	};
}