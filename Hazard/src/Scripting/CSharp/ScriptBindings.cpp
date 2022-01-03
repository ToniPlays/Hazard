
#include "hzrpch.h"
#include "ScriptBindings.h"
#include "Bindings.h"

#include <mono/jit/jit.h>

namespace Hazard::Scripting::CSharp {

	void ScriptBindings::Init()
	{
		using namespace Bindings;
		//Debug bindings
		{
			BindRegistry(ConsoleBindings::GetBindings());
			BindRegistry(GizmoBindings::GetBindings());
		}
		//Application bindings 
		{
			BindRegistry(ApplicationBindings::GetBindings());
			BindRegistry(InputBindings::GetBindings());
		}
		//Component bindings 
		{
			BindRegistry(EntityBindings::GetBindings());
			BindRegistry(TagComponentBindings::GetBindings());
			BindRegistry(TransformBindings::GetBindings());
			BindRegistry(AudioSourceBindings::GetBindings());
		}
		{
			BindRegistry(MathBindings::GetBindings());
		}
		{
			BindRegistry(BufferBindings::GetBindings());
		}
	}
	void ScriptBindings::BindRegistry(BindMap bindings)
	{
		for (auto& [name, method] : bindings) {
			mono_add_internal_call(name, method);
		}
	}
}
