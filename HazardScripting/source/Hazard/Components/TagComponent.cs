using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TagComponent : Component
    {
        public string Name
        {
            get => InternalCalls.Tag_GetName_Native(parentEntity.ID);
            set => InternalCalls.Tag_SetName_Native(parentEntity.ID, value);
        }
    }
}
