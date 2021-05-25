#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {

	class MathBindings {
	public:
		static BindMap GetBindings();
	private:
		static float Math_Random_Native();
		static float Math_RandomRange_Native(float min, float max);
		static float Math_Pow_Native(float value, float pow);
		static float Math_Sqrt_Native(float value);
	};
}