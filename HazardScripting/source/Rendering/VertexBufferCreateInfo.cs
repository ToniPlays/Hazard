using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public enum BufferUsage 
    { 
        StreamDraw = 0,
        StreamRead = 1,
        StreamCopy = 2,
        StaticDraw = 3,
        StaticRead = 4,
        StaticCopy = 5,
        DynamicDraw = 6,
        DynamicRead = 7,
        DynamicCopy = 8
    }
    [StructLayout(LayoutKind.Sequential)]
    public struct VertexBufferCreateInfo
    {
        public ulong Size;
        public BufferUsage Usage;
        public byte[] Data;
    }
}
