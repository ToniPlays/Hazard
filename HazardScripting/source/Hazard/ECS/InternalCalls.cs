using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Coral.Managed.Interop;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        #region Entity
        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32> Entity_IsUpdated_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32, void> Entity_SetUpdate_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32> Entity_IsVisible_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32, void> Entity_SetVisible_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Type, Bool32> Entity_HasComponent_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Type, Bool32> Entity_CreateComponent_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<string, ulong> Entity_InstantiateOrigin_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<string, Vector3, Vector3, Vector3, ulong> Entity_InstantiateAt_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32> Entity_Destroy_Native;
        #endregion
        #region BaseComponent

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Type, Bool32> Component_IsActive_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32, Type, void> Component_SetActive_Native;
        #endregion
        #region World
        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Type, ulong[]> World_GetEntitiesWith_Native;
        #endregion
        #region WorldManager
        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Bool32> WorldManager_LoadWorld_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<string, Bool32> WorldManager_LoadWorldFromString_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong> WorldManager_GetCurrentWorld_Native;
        #endregion
    }
}
