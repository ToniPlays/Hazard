﻿using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TagComponent : Component
    {
        public TagComponent() : base(0) { }
        internal TagComponent(ulong ID) : base(ID) { }
        public string Name
        {
            get
            {
                unsafe
                {
                    return InternalCalls.TagComponent_GetName_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.TagComponent_SetName_Native(ParentEntity.ID, value);
                }
            }
        }
    }
}
