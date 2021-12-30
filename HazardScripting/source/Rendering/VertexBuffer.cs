using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class VertexBuffer
    {
        private ulong ID;

        protected VertexBuffer() { }

        ~VertexBuffer() 
        {
            VertexBuffer_Destroy_Native(ID);
        }

        public void SetData(byte[] data) { }
        public ulong GetSize() { return VertexBuffer_GetSize_Native(ID); }

        public static VertexBuffer Create(VertexBufferCreateInfo info) 
        {
            ulong resourceID = VertexBuffer_Create_Native(ref info);

            VertexBuffer buffer = new VertexBuffer
            {
                ID = resourceID
            };
            return buffer;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong VertexBuffer_GetSize_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong VertexBuffer_Create_Native(ref VertexBufferCreateInfo info);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void VertexBuffer_Destroy_Native(ulong id);
    }
}
