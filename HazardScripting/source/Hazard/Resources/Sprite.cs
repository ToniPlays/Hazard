using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class Sprite : Asset
    {
        public Sprite() : base(0) { }
        internal Sprite(ulong id) : base(id) { }
    }
}
