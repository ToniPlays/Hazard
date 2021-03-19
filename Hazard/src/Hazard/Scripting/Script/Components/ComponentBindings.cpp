#include "hzrpch.h"
#include "ComponentBindings.h"

namespace Hazard::Scripting::Bindings {

    BindMap ComponentBindings::GetBindings()
    {
        BindMap bindings;
        //Tag component
        {
            bindings.emplace_back("Hazard.Entity::Entity_Get_Name_Native", ComponentBindings::GetEntityName);
            bindings.emplace_back("Hazard.Entity::Entity_Set_Name_Native", ComponentBindings::SetEntityName);
        }
        return bindings;
    }
    MonoString* ComponentBindings::GetEntityName(uint64_t id) {
        HZR_CORE_INFO("Entity name is something {0}", id);
        MonoString* result;
        return result;
    }
    void ComponentBindings::SetEntityName(uint64_t id, MonoString* value) {
        HZR_CORE_INFO("Setting entity {0} name {1}", id, ScriptUtils::MonoStringToString(value));
    }
}