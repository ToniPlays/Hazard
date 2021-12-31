using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class Material : Asset
    {
        public Shader shader {
            get { 
                Material_GetShader_Native(out Shader shader);
                return shader;
            }
            
            set => Material_SetShader_Native(ref value);
        }
        [MethodImpl(MethodImplOptions.InternalCall)] //TODO: LINK
        private static extern bool Material_GetShader_Native(out Shader shader);
        [MethodImpl(MethodImplOptions.InternalCall)] //TODO: LINK
        private static extern bool Material_SetShader_Native(ref Shader shader);
    }
}
