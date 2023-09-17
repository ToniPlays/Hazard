using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class IndexBuffer : Buffer
    {
        protected IndexBuffer() { }
        ~IndexBuffer()
        {
            unsafe
            {
                InternalCalls.IndexBuffer_Destroy_Native(ID);
            }
        }
        public void SetData(uint[] data) { }
        public uint GetSize()
        {
            unsafe
            {
                return InternalCalls.IndexBuffer_GetSize_Native(ID);
            }
        }
        public static IndexBuffer Create(IndexBufferCreateInfo info)
        {
            unsafe
            {
                ulong resourceID = InternalCalls.IndexBuffer_Create_Native(info);
                IndexBuffer buffer = new IndexBuffer()
                {
                    ID = resourceID
                };

                return buffer;
            }
        }
    }
}
