using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Resolution
    {
        public float Width;
        public float Height;
        public float RefreshRate;

        public override string ToString()
        {
            return string.Format("{0}x{1} ({2}hz)", Width, Height, RefreshRate);
        }
    }
}
