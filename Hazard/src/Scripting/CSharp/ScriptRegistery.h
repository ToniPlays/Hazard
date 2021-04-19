#pragma once

#include "Hazard/Core/Core.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoType MonoType;
	typedef struct _MonoString MonoString;
}

namespace Hazard::Scripting::CSharp {

	using BindMap = std::vector<std::pair<const char*, void*>>;

	class ScriptRegistery {
	public:
		static void Init();
		static void BindRegistery(BindMap bindings);
	};
}