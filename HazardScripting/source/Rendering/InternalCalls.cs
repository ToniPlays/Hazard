﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Hazard.Rendering;

namespace Hazard
{
    internal partial class InternalCalls
    {
        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint IndexBuffer_GetSize_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong IndexBuffer_Create_Native(ref IndexBufferCreateInfo info);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void IndexBuffer_Destroy_Native(ulong id);


        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong VertexBuffer_GetSize_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong VertexBuffer_Create_Native(ref VertexBufferCreateInfo info);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void VertexBuffer_Destroy_Native(ulong id);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Mesh_Create_Native(ref MeshCreateInfoInternal info);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Mesh_Destroy_Native(ulong id);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong EnvironmentMap_GetCubemapTexture_Native(ulong id, int type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void EnvironmentMap_SetCubemapTexture_Native(ulong id, int type, ulong handle);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong EnvironmentMap_Create_Native();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void EnvironmentMap_Destroy_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void EnvironmentMap_GenerateRadiance_Native(ulong id, ulong sourceImage);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void EnvironmentMap_GenerateIrradiance_Native(ulong id, ulong cubemap);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void EnvironmentMap_GeneratePrefilter_Native(ulong id, ulong cubemap);

    }
}
