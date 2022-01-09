using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public struct IndexBufferCreateInfo {
        public uint Size;
        public BufferUsage Usage;
        public uint[] Data;
    }
}
