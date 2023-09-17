
#include "ScriptAssembly.h"
#include "File.h"

namespace HazardScript
{
	bool ScriptAssembly::LoadAssembly(Coral::HostInstance& host, Coral::AssemblyLoadContext& context)
	{
		std::string path = File::GetFileAbsolutePath(m_Path).lexically_normal().string();
		m_Host = host;
		m_Assembly = context.LoadAssembly(path);

		return m_Assembly.GetLoadStatus() == Coral::AssemblyLoadStatus::Success;
	}
	
	void ScriptAssembly::AddInternalCall(std::string_view className, std::string_view functionName, void* funcPtr)
	{
		m_Assembly.AddInternalCall(className, functionName, funcPtr);
	}

	void ScriptAssembly::UploadInternalCalls()
	{
		m_Assembly.UploadInternalCalls();
	}
}
