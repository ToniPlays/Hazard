using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity
    {
        public ulong ID { get; private set; }

        public string name {
            get => Entity_Get_Name_Native(ID);
            set => Entity_Set_Name_Native(ID, value);
        }

        protected Entity() { ID = 0; }

        internal Entity(ulong ID) {
            this.ID = ID;
        }

        ~Entity() { }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Entity_Get_Name_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Entity_Set_Name_Native(ulong id, string newName);
    }
}
