#pragma once

#include "Core/Core.h"
#include "Maths/Color.h"
#include <hzrpch.h>

namespace Hazard {
	class HAZARD_API GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() const = 0;
		virtual void ClearFrame() const = 0;
		virtual void SetClearColor(Color color) = 0;
		virtual void SetViewport(int x, int y, int w, int h) const = 0;
		virtual double GetMSTime() const = 0;
		virtual Color GetClearColor() = 0;
		virtual std::string GetVersion() const = 0;
		static GraphicsContext* CreateContext(void* window, void* props);

	protected:
		std::string name;
		std::string version;
		Color clearColor;
	};
}