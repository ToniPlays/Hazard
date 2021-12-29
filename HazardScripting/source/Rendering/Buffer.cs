using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard.Rendering
{
    public class Buffer
    {
        private ulong ID;

        internal Buffer(ulong assetID) 
        {
            ID = assetID;
        }

        public void SetData(byte[] data) { }
        public static Buffer Create() { return null; }
    }
}
