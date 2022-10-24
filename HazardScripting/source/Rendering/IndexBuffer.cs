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
            InternalCalls.IndexBuffer_Destroy_Native(ID);
        }
        public void SetData(uint[] data) { }
        public uint GetSize() { return InternalCalls.IndexBuffer_GetSize_Native(ID); }
        public static IndexBuffer Create(IndexBufferCreateInfo info)
        {
            ulong resourceID = InternalCalls.IndexBuffer_Create_Native(ref info);
            IndexBuffer buffer = new IndexBuffer()
            {
                ID = resourceID
            };

            return buffer;
        }
    }
}
