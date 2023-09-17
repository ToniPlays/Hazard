using System;
using System.Collections;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Entity : Reference
    {
        public Entity() : base(0) { }
        internal Entity(ulong ID) : base(ID) { }

        public string Name
        {
            get => Get<TagComponent>().Name;
            set => Get<TagComponent>().Name = value;
        }
        public TransformComponent Transform
        {
            get => Get<TransformComponent>();
        }

        public bool IsUpdated() { unsafe { return InternalCalls.Entity_IsUpdated_Native(ID); } }
        public void SetUpdate(bool update) { unsafe { InternalCalls.Entity_SetUpdate_Native(ID, update); } }
        public bool IsVisible() { unsafe { return InternalCalls.Entity_IsVisible_Native(ID); } }
        public void SetVisible(bool visible) { unsafe { InternalCalls.Entity_SetVisible_Native(ID, visible); } }
        public T Get<T>() where T : Component, new()
        {
            if (!Has<T>())
                return null;

            T component = new T
            {
                ParentEntity = this
            };

            return component;
        }
        public bool Has<T>() where T : Component, new()
        {
            unsafe
            {
                return InternalCalls.Entity_HasComponent_Native(ID, typeof(T));
            }
        }
        public T Create<T>() where T : Component, new()
        {
            unsafe
            {
                InternalCalls.Entity_CreateComponent_Native(ID, typeof(T));

                T component = new T
                {
                    ParentEntity = this
                };
                return component;
            }
        }

        public Entity Instantiate(string name = "New entity")
        {
            unsafe
            {
                ulong id = InternalCalls.Entity_InstantiateOrigin_Native(name);
                return new Entity(id);
            }
        }
        public Entity Instantiate(string name, Vector3 position, Vector3 rotation, Vector3 scale)
        {
            unsafe
            {
                ulong id = InternalCalls.Entity_InstantiateAt_Native(name, position, rotation, scale);
                return new Entity(id);
            }
        }
        public void Destroy() { unsafe { InternalCalls.Entity_Destroy_Native(ID); } }
    }
}
