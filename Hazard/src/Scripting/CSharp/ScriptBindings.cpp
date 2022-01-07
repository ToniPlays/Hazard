
#include "hzrpch.h"
#include "ScriptBindings.h"
#include "Bindings.h"

#include <mono/jit/jit.h>

#define BIND_REGISTRY(x) BindRegistry(x::GetBindings());

namespace Hazard::Scripting::CSharp {

	void ScriptBindings::Init()
	{
		using namespace Bindings;
		//Debug bindings
		{
			BIND_REGISTRY(ConsoleBindings);
			BIND_REGISTRY(GizmoBindings);
		}
		//Application bindings 
		{
			BIND_REGISTRY(ApplicationBindings);
			BIND_REGISTRY(InputBindings);
		}
		//Component bindings 
		{
			BIND_REGISTRY(EntityBindings);
			BIND_REGISTRY(TagComponentBindings);
			BIND_REGISTRY(TransformBindings);
			BIND_REGISTRY(AudioSourceBindings);
			BIND_REGISTRY(MeshComponentBindings);
		}
		//Utility
		{
			BIND_REGISTRY(MathBindings);
		}
		//Rendering
		{
			BIND_REGISTRY(BufferBindings);
			BIND_REGISTRY(MeshBindings);
		}
	}
	void ScriptBindings::BindRegistry(BindMap bindings)
	{
		for (auto& [name, method] : bindings) {
			mono_add_internal_call(name, method);
		}
	}
}
