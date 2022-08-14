﻿using System;
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
        public string Name {
            get => Get<TagComponent>().Name;
            set => Get<TagComponent>().Name = value;
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
                parentEntity = this
            };

            return component;
        }
        public bool Has<T>() where T : Component, new() 
        {
            return InternalCalls.Entity_HasComponent_Native(ID, typeof(T));
        }
        public T Create<T>() where T : Component, new() 
        {
            InternalCalls.Entity_CreateComponent_Native(ID, typeof(T));

            T component = new T
            {
                parentEntity = this
            };
            return component;
        }

        public Entity Instantiate(string name = "New entity") 
        {
            ulong id = InternalCalls.Entity_InstantiateOrigin_Native(name);
            return new Entity() { ID = id };
        }
        public Entity Instantiate(string name, Vector3 position, Vector3 rotation, Vector3 scale)
        {
            ulong id = InternalCalls.Entity_InstantiateAt_Native(name, position, rotation, scale);
            return new Entity() { ID = id };
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