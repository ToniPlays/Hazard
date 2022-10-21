using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public abstract class Component : Reference
    {
        internal Component(ulong id) : base(id) { }
        public Entity ParentEntity
        {
            get { return new Entity(ID); }
            internal set { ID = value.ID; }
        }
    }
}
