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
        #region Debug
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Log_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Info_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Warn_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Error_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Critical_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Trace_Native(object message);
        #endregion
        #region ECS 
        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_GetActive_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetActive_Native(ulong id, bool active);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent_Native(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_CreateComponent_Native(ulong id, Type type);
        #endregion
        #region TransformComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetPosition_Native(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetPosition_Native(ulong id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetRotation_Native(ulong id, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetRotation_Native(ulong id, ref Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetScale_Native(ulong id, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetScale_Native(ulong id, ref Vector3 scale);
        #endregion
        #endregion
    }
}
