﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class SpriteRendererComponent : Component
    {
        public SpriteRendererComponent() : base(0) { }
        internal SpriteRendererComponent(ulong ID) : base(ID) {}

        public Color Color { 
            get {
                InternalCalls.SpriteRendererComponent_GetColor_Native(parentEntity.ID, out Color result);
                return result;
            }
            set => InternalCalls.SpriteRendererComponent_SetColor_Native(parentEntity.ID, ref value);
        }
        public bool IsActive() { return InternalCalls.Component_IsActive_Native(parentEntity.ID, typeof(SpriteRendererComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(parentEntity.ID, active, typeof(SpriteRendererComponent)); }
    }
}