using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class Reference
    {
        protected Reference() { ID = 0; }
        internal Reference(ulong ID)
        {
            this.ID = ID;
        }
        ~Reference() {}

        public ulong ID { get; protected set; }
    }
}
