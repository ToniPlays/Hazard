using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class SpriteRendererComponent : Component
    {
        public SpriteRendererComponent() : base(0) { }
        internal SpriteRendererComponent(ulong ID) : base(ID) { }

        public Color Color
        {
            get
            {
                unsafe
                {
                    return InternalCalls.SpriteRendererComponent_GetColor_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.SpriteRendererComponent_SetColor_Native(ParentEntity.ID, value);
                }
            }
        }
        public Sprite Sprite
        {
            get
            {
                unsafe
                {
                    return new Sprite(InternalCalls.SpriteRendererComponent_GetSprite_Native(ParentEntity.ID));
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.SpriteRendererComponent_SetSprite_Native(ParentEntity.ID, value.ID);
                }
            }
        }
        public bool IsActive()
        {
            unsafe
            {
                return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(SpriteRendererComponent));
            }
        }
        public void SetActive(bool active)
        {
            unsafe
            {
                InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(SpriteRendererComponent));
            }
        }
    }
}
