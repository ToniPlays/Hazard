﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Hazard.Rendering
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vertex3D 
    {
        public Vector3 Position;
        public Color Color;
        public Vector3 Normal;
        public Vector2 TextureCoords;
    }

    public struct MeshCreateInfo {
        public VertexBuffer VertexBuffer;
        public IndexBuffer IndexBuffer;
    }

    internal struct MeshCreateInfoInternal {
        public ulong VertexID;
        public ulong IndexID;
    }
    public class Mesh
    {
        private ulong ID;
        protected Mesh() { }

        ~Mesh() {
            Mesh_Destroy_Native(ID);
        }
        public static Mesh Create(MeshCreateInfo info) { 
            MeshCreateInfoInternal internalInfo = new MeshCreateInfoInternal();
            internalInfo.VertexID = info.VertexBuffer.GetID();
            internalInfo.IndexID = info.IndexBuffer.GetID();

            ulong resourceID = Mesh_Create_Native(ref internalInfo);

            Mesh mesh = new Mesh()
            {
                ID = resourceID
            };
            return mesh;
        }

        internal ulong GetID() { return ID; }
        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Mesh_Create_Native(ref MeshCreateInfoInternal info);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Mesh_Destroy_Native(ulong id);
        #endregion
    }
}