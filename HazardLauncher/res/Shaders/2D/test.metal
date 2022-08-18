//
//  test.metal
//  HazardEditor
//
//  Created by Toni Simoska on 5.1.2022.
//

#include <metal_stdlib>
using namespace metal;

vertex float4 basic_vertex(                           // 1
  const device packed_float3* vertex_array [[ buffer(0) ]], // 2
  unsigned int vid [[ vertex_id ]]) {                 // 3
  return float4(vertex_array[vid], 1.0);              // 4
}

fragment half4 basic_fragment() { // 1
  return half4(1.0);              // 2
}
