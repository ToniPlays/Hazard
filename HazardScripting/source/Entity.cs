using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity
    {
        public ulong ID { get; private set; }
        protected Entity() { ID = 0; }

        internal Entity(ulong ID) {
            this.ID = ID;
        }
        ~Entity() { }
    }
}
