using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class Gizmo
    {
        public static void DrawLine(Vector3 start, Vector3 end, Color color) { Gizmo_DrawLine_Native(start, end, color); }

        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Gizmo_DrawLine_Native(Vector3 start, Vector3 end, Color color);
        #endregion
    }
}
