#include <simd/simd.h>

struct VertexIn {
    const packed_float3 a_Position;
    const packed_float4 a_Color;
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut Vertex (
    const device VertexIn* vertices[[buffer(0)]],
    uint vid [[vertex_id]]) {
        
        VertexIn input = vertices[vid];
        
        VertexOut out;
        out.position = float4(input.a_Position, 1.0);
        out.color = float4(input.a_Color);
    
        return out;
}
#include <simd/simd.h>


fragment half4 Fragment(VertexOut in [[stage_in]]) {
    return half4(in.color);
}

