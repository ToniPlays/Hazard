#pragma once

namespace Hazard
{
	class GUIPlatform
	{
	public:
		GUIPlatform() = default;
		virtual ~GUIPlatform() = default;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Close() = 0;
	};
}