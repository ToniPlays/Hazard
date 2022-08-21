using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class VertexBuffer : Reference
    {
        protected VertexBuffer() { }

        ~VertexBuffer() 
        {
            InternalCalls.VertexBuffer_Destroy_Native(ID);
        }

        public void SetData(byte[] data) { }
        public ulong GetSize() { return InternalCalls.VertexBuffer_GetSize_Native(ID); }

        public static VertexBuffer Create(VertexBufferCreateInfo info) 
        {
            info.Size *= 12 * 4;
            ulong resourceID = InternalCalls.VertexBuffer_Create_Native(ref info);

            VertexBuffer buffer = new VertexBuffer
            {
                ID = resourceID
            };
            return buffer;
        }
    }
}
