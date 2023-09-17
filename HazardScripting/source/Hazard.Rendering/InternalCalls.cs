using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Hazard.Rendering;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, uint> IndexBuffer_GetSize_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<IndexBufferCreateInfo, ulong> IndexBuffer_Create_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, void> IndexBuffer_Destroy_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong> VertexBuffer_GetSize_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<VertexBufferCreateInfo, ulong> VertexBuffer_Create_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong> VertexBuffer_Destroy_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<MeshCreateInfoInternal, ulong> Mesh_Create_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong> Mesh_Destroy_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, int, ulong> EnvironmentMap_GetCubemapTexture_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, int, ulong, void> EnvironmentMap_SetCubemapTexture_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong> EnvironmentMap_Create_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, void> EnvironmentMap_Destroy_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong, void> EnvironmentMap_GenerateRadiance_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong, void> EnvironmentMap_GenerateIrradiance_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<ulong, ulong, void> EnvironmentMap_GeneratePrefilter_Native;

    }
}
