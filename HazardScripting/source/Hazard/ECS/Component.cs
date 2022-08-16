using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public abstract class Component
    {
        internal Component(ulong id) 
        {
            parentEntity = new Entity(id);
        }
        public Entity parentEntity { get; set; }

        public bool IsActive() { return InternalCalls.Component_IsActive_Native(parentEntity.ID); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(parentEntity.ID, active); }

    }
}
