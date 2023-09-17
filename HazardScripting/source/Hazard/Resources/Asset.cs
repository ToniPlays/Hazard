using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public class Asset : Reference
    {
        public Asset() : base(0) { }
        internal Asset(ulong id) : base(id) { }

        public string GetName()
        {
            unsafe
            {
                return InternalCalls.Asset_GetName_Native(ID);
            }
        }
    }
}
