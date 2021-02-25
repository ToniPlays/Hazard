#pragma once
#include "Core.h"

namespace Hazard::Module {
	class Module {
	public:
		Module(const char* name) : name(name) {}
		~Module() {}
		

		virtual void PreInit() {}
		virtual void Init() {}
		virtual void Update() {}
		virtual void Render() {}
		virtual void Close() {}


		bool GetActive() { return isActive; }
		void SetActive(bool active) { isActive = active; }

		std::string GetName() { return name; };
	private:
		bool isActive = false;
		std::string name;
	};
}