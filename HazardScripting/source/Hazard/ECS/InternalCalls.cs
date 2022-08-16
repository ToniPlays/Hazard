using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    internal partial class InternalCalls
    {
        #region Entity
        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_IsActive_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetActive_Native(ulong id, bool active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent_Native(ulong id, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_CreateComponent_Native(ulong id, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Entity_InstantiateOrigin_Native(string id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Entity_InstantiateAt_Native(string id, Vector3 position, Vector3 rotation, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_Destroy_Native(ulong id);
        #endregion
        #region BaseComponent


        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Component_IsActive_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void  Component_SetActive_Native(ulong id, bool active);

        #endregion
    }
}
