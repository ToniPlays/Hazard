using System;
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

        public static uint Size() { return 48; }
    }

    public struct MeshCreateInfo
    {
        public VertexBuffer VertexBuffer;
        public IndexBuffer IndexBuffer;
    }

    internal struct MeshCreateInfoInternal
    {
        public ulong VertexID;
        public ulong IndexID;
    }
    public class Mesh : Asset
    {
        internal Mesh(ulong id) : base(id) { }
        protected Mesh() { }

        ~Mesh()
        {
            unsafe
            {
                InternalCalls.Mesh_Destroy_Native(ID);
            }
        }
        public static Mesh Create(MeshCreateInfo info)
        {
            unsafe
            {
                MeshCreateInfoInternal internalInfo = new MeshCreateInfoInternal();
                internalInfo.VertexID = info.VertexBuffer.ID;
                internalInfo.IndexID = info.IndexBuffer.ID;

                ulong resourceID = InternalCalls.Mesh_Create_Native(internalInfo);

                Mesh mesh = new Mesh()
                {
                    ID = resourceID
                };
                return mesh;
            }
        }
    }
}
