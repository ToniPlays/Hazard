using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity
    {
        protected Entity() { ID = 0; }
        ~Entity() { }

        public ulong ID { get; private set; }
        public string name {
            get => Get<TagComponent>().name;
            set => Get<TagComponent>().name = value;
        }
        public bool active
        {
            get => Entity_Get_Active_Native(ID);
            set => Entity_Set_Active_Native(ID, value);
        }
        public T Get<T>() where T : Component, new()
        {
            if (!Has<T>()) 
                return null;

            T component = new T();
            component.parent = this;

            return component;
        }
        public bool Has<T>() where T : Component, new() {
            return Entity_HasComponent_Native(ID, typeof(T));
        }
        
        internal Entity(ulong ID) {
            this.ID = ID;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Entity_Get_Active_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Entity_Set_Active_Native(ulong id, bool active);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Entity_HasComponent_Native(ulong id, Type type);
    }
}
