
// Used in PBR shader
struct PBRParameters
{
	vec3 Albedo;
	float Roughness;
	float Metalness;

	vec3 Normal;
	vec3 View;
	float NdotV;
} m_Params;


//PBR calculations
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return nom / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0), F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 CalculateDirectionalLight(vec3 F0, DirectionalLight light) 
{
	vec3 L = light.Direction.xyz;
	vec3 H = normalize(m_Params.View + L);

	vec3 radiance = light.Color.rgb * light.Color.a;
	//-------
	float NdotL = max(dot(m_Params.Normal, L), 0.0);

	float NDF	= DistributionGGX(m_Params.Normal, H, m_Params.Roughness);
	float G		= GeometrySmith(m_Params.Normal, m_Params.View, L, m_Params.Roughness);
	vec3 F		= FresnelSchlick(max(dot(H, m_Params.View), 0.0), F0);
		
	vec3 numerator		= NDF * G * F;
	float denominator	= 4.0 * m_Params.NdotV * NdotL + Epsilon;
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
		
	kD *= 1.0 - m_Params.Metalness;

	return (kD * m_Params.Albedo / PI + specular) * radiance * NdotL;
}