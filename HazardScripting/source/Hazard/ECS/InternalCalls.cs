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
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_IsUpdated_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetUpdate_Native(ulong id, bool active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_IsVisible_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetVisible_Native(ulong id, bool active);


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


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Component_IsActive_Native(ulong id, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Component_SetActive_Native(ulong id, bool active, Type type);
        #endregion

        #region WorldManager
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool WorldManager_LoadWorld_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool WorldManager_LoadWorldFromString_Native(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong WorldManager_GetCurrentWorld_Native();
        #endregion
    }
}
