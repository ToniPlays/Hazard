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
        float Scale;
    }

    internal struct MeshCreateInfoInternal
    {
        public ulong VertexID;
        public ulong IndexID;
    }
}
