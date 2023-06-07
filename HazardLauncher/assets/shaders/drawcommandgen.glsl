#type Compute

#version 450 core

#extension GL_EXT_shader_atomic_float : require
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

struct DrawCommand
{
    uint    IndexCount;
    uint    InstanceCount;
    uint    FirstIndex;
    int     VertexOffset;
    uint    FirstInstance;
};

layout(push_constant) uniform PushConstants
{
    uint64_t Input;
    uint64_t Output;
} pushConstants;

layout(set = 0, binding = 1) buffer InDrawCommands { DrawCommand dc[]; } cullIn;
layout(set = 0, binding = 2) buffer OutDrawCommands { DrawCommand dc[]; } cullOut;

layout(local_size_x = 1, local_size_y = 1, local_size_y = 1) in;
void main()
{
    DrawCommand object = cullIn.dc[gl_GlobalInvocationID.x];


    uint indicesToDraw = object.IndexCount;
    uint drawId = atomicAdd(cullOut.dc[0].IndexCount, 1);

    cullOut.dc[drawId + 1] = DrawCommand(
        indicesToDraw, 1, 0, 0, gl_GlobalInvocationID.x);
}
