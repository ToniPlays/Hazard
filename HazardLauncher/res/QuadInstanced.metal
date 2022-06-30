#include <simd/simd.h>

using namespace metal;

struct VertexIn {
    const packed_float3 a_Position;
};
struct InstancedData {
    const packed_float4 a_Color;
    const packed_float4 a_MRow0;
    const packed_float4 a_MRow1;
    const packed_float4 a_MRow2;
};

struct Camera
{
    float4x4 ViewProjection;
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut Vertex (
    const device VertexIn* vertices[[buffer(0)]],
    const device InstancedData* instanceData[[buffer(1)]],
    constant Camera& u_Camera[[buffer(2)]],
    uint vid [[vertex_id]],
    uint iid [[instance_id]]) {
        
        VertexIn input = vertices[vid];
        InstancedData data = instanceData[iid];
        
        float4x4 transform = {
            float4(data.a_MRow0.x, data.a_MRow1.x, data.a_MRow2.x, 0.0),
            float4(data.a_MRow0.y, data.a_MRow1.y, data.a_MRow2.y, 0.0),
            float4(data.a_MRow0.z, data.a_MRow1.z, data.a_MRow2.z, 0.0),
            float4(data.a_MRow0.w, data.a_MRow1.w, data.a_MRow2.w, 1.0)
            
        };
        
        VertexOut out;
        out.position = u_Camera.ViewProjection * transform * float4(input.a_Position, 1.0);
        out.color = data.a_Color;
    
        return out;
}


fragment half4 Fragment(VertexOut in [[stage_in]]) {
    return half4(in.color);
}


