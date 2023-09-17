using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, string> Asset_GetName_Native;
    }
}
