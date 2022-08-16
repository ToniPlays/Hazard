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
    }
}
