using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class VertexBuffer : Buffer
    {
        protected VertexBuffer() { }

        ~VertexBuffer()
        {
            unsafe
            {
                InternalCalls.VertexBuffer_Destroy_Native(ID);
            }
        }

        public unsafe void SetData(IntPtr* data) { }
        public ulong GetSize()
        {
            unsafe
            {
                return InternalCalls.VertexBuffer_GetSize_Native(ID);
            }
        }

        public static VertexBuffer Create(VertexBufferCreateInfo info)
        {
            unsafe
            {
                info.Size *= Vertex3D.Size();
                ulong resourceID = InternalCalls.VertexBuffer_Create_Native(info);

                VertexBuffer buffer = new VertexBuffer
                {
                    ID = resourceID
                };
                return buffer;
            }
        }
    }
}
