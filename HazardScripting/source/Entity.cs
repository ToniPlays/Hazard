using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity
    {
        protected Entity() { ID = 0; }
        //TODO: LINK
        internal Entity(ulong ID)
        {
            this.ID = ID;
        }
        ~Entity() { }

        public ulong ID { get; private set; }
        //TODO: LINK
        public string name {
            get => Get<TagComponent>().name;
            set => Get<TagComponent>().name = value;
        }
        //TODO: LINK
        public TransformComponent transform
        {
            get => Get<TransformComponent>();
        }
        //TODO: LINK
        public bool IsActive() { return Entity_GetActive_Native(ID); }
        //TODO: LINK
        public void SetActive(bool active) { Entity_SetActive_Native(ID, active); }
        //TODO: LINK
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
        //TODO: LINK
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
