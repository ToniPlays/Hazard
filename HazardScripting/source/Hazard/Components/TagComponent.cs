using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TagComponent : Component
    {
        public TagComponent() : base(0) { }
        internal TagComponent(ulong ID) : base(ID) {}
        public string Name
        {
            get => InternalCalls.TagComponent_GetName_Native(parentEntity.ID);
            set => InternalCalls.TagComponent_SetName_Native(parentEntity.ID, value);
        }
    }
}
