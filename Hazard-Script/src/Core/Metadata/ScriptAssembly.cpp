
#include "ScriptAssembly.h"
#include "Filesystem/File.h"

namespace HazardScript
{
	bool ScriptAssembly::LoadAssembly(Coral::HostInstance& host, Coral::AssemblyLoadContext& context)
	{
		if (!File::Exists(m_Path)) return false;

		std::string path = File::GetFileAbsolutePath(m_Path).lexically_normal().string();
		m_Host = host;
		m_Assembly = context.LoadAssembly(path);
        
		bool loaded = m_Assembly.GetLoadStatus() == Coral::AssemblyLoadStatus::Success;
        HZR_VERIFY(loaded, "Assembly loaded");
        return loaded;
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
