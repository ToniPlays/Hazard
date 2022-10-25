
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
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2
float NdfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float GaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float GaSchlickGGX(float cosLi, float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return GaSchlickG1(cosLi, k) * GaSchlickG1(NdotV, k);
}


vec3 CalculateDirectionalLight(vec3 F0, DirectionalLight light) 
{
	
	vec3 Li = light.Direction.xyz;
	vec3 Lradiance = light.Color.rgb * light.Color.a;
	vec3 Lh = normalize(Li + m_Params.View);

	float cosLi = max(0.0, dot(m_Params.Normal, Li));
	float cosLh = max(0.0, dot(m_Params.Normal, Lh));

	vec3 F = FresnelSchlickRoughness(max(0.0, dot(Lh, m_Params.View)), F0, m_Params.Roughness);
	float D = NdfGGX(cosLh, m_Params.Roughness);
	float G = GaSchlickGGX(cosLi, m_Params.NdotV, m_Params.Roughness);

	vec3 kD = (1.0 - F) * (1.0 - m_Params.Metalness);
	vec3 diffuseBRDF = kD * m_Params.Albedo;

	vec3 specularBRDF = (F * D * G) / max(0.0000001, 4.0 * cosLi * m_Params.NdotV);
	specularBRDF = clamp(specularBRDF, vec3(0.0), vec3(10.0));
	return (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
}