using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class Image2D : Asset
    {
        public Image2D() : base(0) { }
        internal Image2D(ulong ID) : base(ID) { }
    }
}
