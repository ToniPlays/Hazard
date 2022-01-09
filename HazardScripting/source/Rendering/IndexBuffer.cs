using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class IndexBuffer
    {
        private ulong ID;
        protected IndexBuffer() { }
        ~IndexBuffer() {
            IndexBuffer_Destroy_Native(ID);
        }
        public void SetData(uint[] data) { }
        public uint GetSize() { return IndexBuffer_GetSize_Native(ID); }
        public static IndexBuffer Create(IndexBufferCreateInfo info)
        {
            ulong resourceID = IndexBuffer_Create_Native(ref info);
            IndexBuffer buffer = new IndexBuffer()
            {
                ID = resourceID
            };

            return buffer;
        }
        internal ulong GetID() { return ID; }

        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint IndexBuffer_GetSize_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong IndexBuffer_Create_Native(ref IndexBufferCreateInfo info);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void IndexBuffer_Destroy_Native(ulong id);
        #endregion

    }
}
