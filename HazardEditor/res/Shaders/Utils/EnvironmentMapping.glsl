
vec3 GetCubeMapTextureCoord(vec2 imageSize) 
{
	vec2 st = gl_GlobalInvocationID.xy / imageSize;
	vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

	switch(gl_GlobalInvocationID.z) 
	{
		case 0: return vec3( 1.0, uv.y, -uv.x);
		case 1: return vec3(-1.0, uv.y,  uv.x);
		case 2: return vec3( uv.x,  1.0,-uv.y);
		case 3: return vec3( uv.x, -1.0, uv.y);
		case 4: return vec3( uv.x, uv.y, 1.0);
		case 5: return vec3(-uv.x, uv.y,-1.0);
	}
	return vec3(1.0);
}