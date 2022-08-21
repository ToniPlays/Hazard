using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public abstract class Component : Reference
    {
        internal Component(ulong id) : base(id) { }
        public Entity parentEntity
        {
            get { return new Entity(ID); }
            set { ID = value.ID; }
        }
    }
}
