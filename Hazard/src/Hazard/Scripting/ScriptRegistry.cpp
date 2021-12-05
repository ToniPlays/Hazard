#include "hzrpch.h"
#include "ScriptRegistry.h"

namespace Hazard::Scripting {

    bool ScriptRegistry::HasScript(const std::string& script)
    {
        return m_Scripts.find(script) != m_Scripts.end();
    }
    ScriptMetaData& ScriptRegistry::GetScriptMetadata(const std::string& script)
    {
        return m_Scripts[script];
    }
    void ScriptRegistry::RegisterEntityScript(uint32_t handle, Script* script)
    {
        GetScriptMetadata(script->GetModuleName()).Count++;

        InstanceData data;
        data.Handle = handle;
        data.Scripts.push_back(script);
        m_InstanceData[handle] = data;
    }
}