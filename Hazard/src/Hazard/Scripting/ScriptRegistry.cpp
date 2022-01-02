#include <hzrpch.h>
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
        GetScriptMetadata(script->GetModuleName()).Count++;
        InstanceData data;

        data.Handle = handle;
        data.Scripts.push_back(script);
        m_InstanceData[handle] = data;
    }
    void ScriptRegistry::Clear()
    {
        for (auto& [name, metadata] : m_Scripts) {
            for (Attribute* attrib : metadata.ClassAttributes) 
            {
                delete attrib;
            }
            for (ScriptFieldMetadata field : metadata.Fields)
            {
                for (Attribute* attrib : field.FieldAttributes) {
                    delete attrib;
                }
            }
            for (ScriptMethodMetadata method : metadata.Methods)
            {
                for (Attribute* attrib : method.MethodAttributes) {
                    delete attrib;
                }
            }
        }
        m_Scripts.clear();

        for (auto& [handle, instance] : m_InstanceData) 
        {
            for (Script* script : instance.Scripts) {
                delete script;
            }
        }
        HZR_CORE_WARN("Clearing {0} script instances", m_InstanceData.size());
        m_InstanceData.clear();
    }
}
