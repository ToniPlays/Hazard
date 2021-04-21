using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity
    {
        protected Entity() { ID = 0; }
        internal Entity(ulong ID)
        {
            this.ID = ID;
        }
        ~Entity() { }

        public ulong ID { get; private set; }
        public string name {
            get => Get<TagComponent>().name;
            set => Get<TagComponent>().name = value;
        }
        public bool IsActive() { return Entity_GetActive_Native(ID); }
        public void SetActive(bool active) { Entity_SetActive_Native(ID, active); }
        public T Get<T>() where T : Component, new()
        {
            if (!Has<T>()) 
                return null;

            T component = new T
            {
                parent = this
            };

            return component;
        }
        public bool Has<T>() where T : Component, new() {
            return Entity_HasComponent_Native(ID, typeof(T));
        }
        
        

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Entity_GetActive_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Entity_SetActive_Native(ulong id, bool active);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Entity_HasComponent_Native(ulong id, Type type);
    }
}
