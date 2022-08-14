using System;
using System.Collections;
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
        public TransformComponent transform
        {
            get => Get<TransformComponent>();
        }
        public bool IsActive() { return InternalCalls.Entity_GetActive_Native(ID); }
        public void SetActive(bool active) { InternalCalls.Entity_SetActive_Native(ID, active); }
        public T Get<T>() where T : Component, new()
        {
            if (!Has<T>()) return null;

            T component = new T
            {
                parent = this
            };

            return component;
        }
        public bool Has<T>() where T : Component, new() 
        {
            return InternalCalls.Entity_HasComponent_Native(ID, typeof(T));
        }
        public T Add<T>() where T : Component, new() 
        {
            InternalCalls.Entity_CreateComponent_Native(ID, typeof(T));

            T component = new T
            {
                parent = this
            };
            return component;
        }

        public void Destroy() { InternalCalls.Entity_Destroy_Native(ID); }

        public Coroutine BeginCoroutine(IEnumerator enumerator) 
        {
            return null;
        }
        public void StopCoroutine(Coroutine coroutine) 
        { 
            
        }
        public void StopAllCoroutines() 
        { 
            
        }
    }
}
