using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TagComponent : Component
    {
        public string name
        {
            get => InternalCalls.Tag_GetName_Native(parent.ID);
            set => InternalCalls.Tag_SetName_Native(parent.ID, value);
        }
    }
}
