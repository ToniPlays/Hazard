#type Compute

#version 450 core

layout(set = 0, binding = 0, rgba8) restrict writeonly uniform image2D o_OutputImage;

layout(local_size_x = 1, local_size_y = 1, local_size_y = 1) in;
void main()
{
    vec4 value = vec4(1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    
    value.y = 1.0f;
    value.x = float(texelCoord.x) / (gl_NumWorkGroups.x);
    value.z = float(texelCoord.y) / (gl_NumWorkGroups.y);
        
    imageStore(o_OutputImage, texelCoord, value);
}
