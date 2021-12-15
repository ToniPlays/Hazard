#pragma once
#include "hzrpch.h"
#include "ScriptRegistery.h"
#include "Bindings.h"

#include <mono/jit/jit.h>

namespace Hazard::Scripting::CSharp {

	void ScriptRegistery::Init()
	{
		using namespace Bindings;
		//Debug bindings
		{
			BindRegistery(ConsoleBindings::GetBindings());
			BindRegistery(GizmoBindings::GetBindings());
		}
		//Application bindings 
		{
			BindRegistery(ApplicationBindings::GetBindings());
			BindRegistery(InputBindings::GetBindings());
		}
		//Component bindings 
		{
			BindRegistery(EntityBindings::GetBindings());
			BindRegistery(TagComponentBindings::GetBindings());
			BindRegistery(TransformBindings::GetBindings());
			BindRegistery(AudioSourceBindings::GetBindings());
		}
		{
			BindRegistery(MathBindings::GetBindings());
		}
	}
	void ScriptRegistery::BindRegistery(BindMap bindings) 
	{
		for (auto& [name, method] : bindings) {
			mono_add_internal_call(name, method);
		}
	}
}
