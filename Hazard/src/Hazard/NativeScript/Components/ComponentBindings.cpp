#include "hzrpch.h"
#include "ComponentBindings.h"
#include "Hazard/Entity/SceneCommand.h"

namespace Hazard::Scripting::Bindings {

    BindMap ComponentBindings::GetBindings()
    {
        BindMap bindings;
        //Tag component
        {
            bindings.emplace_back("Hazard.Entity::Entity_Get_Name_Native", ComponentBindings::GetEntityName);
            bindings.emplace_back("Hazard.Entity::Entity_Set_Name_Native", ComponentBindings::SetEntityName);

            bindings.emplace_back("Hazard.Entity::Entity_Get_Active_Native", ComponentBindings::GetEntityActive);
            bindings.emplace_back("Hazard.Entity::Entity_Set_Active_Native", ComponentBindings::SetEntityActive);

        }
        return bindings;
    }
    MonoString* ComponentBindings::GetEntityName(uint32_t id) 
    {
        ECS::Entity& entity = ECS::SceneCommand::GetEntity(id);
        return ScriptUtils::StringToMonoString(entity.GetTag().tag.c_str());
    }
    void ComponentBindings::SetEntityName(uint32_t id, MonoString* value) 
    {
        ECS::Entity& entity = ECS::SceneCommand::GetEntity(id);
        entity.GetTag().tag = ScriptUtils::MonoStringToString(value);
    }
    bool ComponentBindings::GetEntityActive(uint32_t id)
    {
        return ECS::SceneCommand::GetEntity(id).GetTag().visible;
    }
    void ComponentBindings::SetEntityActive(uint32_t id, bool value)
    {
        ECS::SceneCommand::GetEntity(id).GetTag().visible = value;
    }
}