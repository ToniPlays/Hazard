using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class VertexBuffer : Resource
    {
        public VertexBuffer() { }

        public void SetData(byte[] data) { }
        public ulong GetSize() { return VertexBuffer_GetSize_Native(ID); }

        public static VertexBuffer Create() 
        {
            return CreateNative<VertexBuffer>();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong VertexBuffer_GetSize_Native(ulong id);
    }
}
