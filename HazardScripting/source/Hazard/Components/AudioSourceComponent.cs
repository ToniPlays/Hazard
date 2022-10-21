using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class AudioSourceComponent : Component
    {
        public AudioSourceComponent() : base(0) { }
        internal AudioSourceComponent(ulong ID) : base(ID) { }

        public bool IsActive() { return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(AudioSourceComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(AudioSourceComponent)); }
    }
}
