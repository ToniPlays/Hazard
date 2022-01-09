using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TagComponent : Component
    {
        public string name
        {
            get => TagComponent_GetName_Native(parent.ID);
            set => TagComponent_SetName_Native(parent.ID, value);
        }
        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string TagComponent_GetName_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TagComponent_SetName_Native(ulong id, string name);
        #endregion
    }
}
