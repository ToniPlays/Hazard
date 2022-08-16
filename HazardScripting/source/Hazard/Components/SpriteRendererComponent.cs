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
        internal SpriteRendererComponent(ulong ID) : base(ID) {}

        public Color Color { 
            get {
                InternalCalls.SpriteRendererComponent_GetColor(parentEntity.ID, out Color result);
                return result;
            }
            set => InternalCalls.SpriteRendererComponent_SetColor(parentEntity.ID, ref value);
        }
    }
}
