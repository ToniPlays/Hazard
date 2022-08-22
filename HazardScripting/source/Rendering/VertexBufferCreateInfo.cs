using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    [StructLayout(LayoutKind.Sequential)]
    public struct VertexBufferCreateInfo
    {
        public BufferUsage Usage;
        public uint Size;
        public Vertex3D[] Data;
    }
}
