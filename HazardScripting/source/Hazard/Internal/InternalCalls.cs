using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    internal class InternalCalls
    {
        #region Application
        //Application class
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_Quit_Native();


        #endregion
    }
}
