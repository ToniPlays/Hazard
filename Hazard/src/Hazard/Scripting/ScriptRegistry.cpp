#include "hzrpch.h"
#include "ScriptRegistry.h"

namespace Hazard::Scripting {

    bool ScriptRegistry::HasScript(const std::string& script)
    {
        return m_Scripts.find(script) != m_Scripts.end();
    }
    bool ScriptRegistry::HasInstance(uint32_t handle)
    {
        return m_InstanceData.find(handle) != m_InstanceData.end();
    }
    ScriptMetadata& ScriptRegistry::GetScriptMetadata(const std::string& script)
    {
        return m_Scripts[script];
    }
    void ScriptRegistry::Remove(uint32_t handle)
    {
        InstanceData& data = GetInstanceData(handle);
        for (auto& script : data.Scripts) 
        {
            GetScriptMetadata(script->GetModuleName()).Count--;
            delete script;
        }
        m_InstanceData.erase(handle);
    }
    void ScriptRegistry::RegisterEntityScript(uint32_t handle, Script* script)
    {
        InstanceData data;
        data.Handle = handle;
        data.Scripts.push_back(script);
        m_InstanceData[handle] = data;
    }
}