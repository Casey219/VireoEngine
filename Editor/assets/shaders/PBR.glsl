#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 4) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float padding;
};

layout(std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	ivec4 u_EntityData;
};

layout(location = 0) out vec3 v_WorldPos;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out mat3 v_TBN;

void main()
{
	vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
	v_WorldPos = worldPos.xyz;
	v_TexCoord = a_TexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
	vec3 N = normalize(normalMatrix * a_Normal);
	vec3 T = normalize(normalMatrix * a_Tangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N, T));
	v_TBN = mat3(T, B, N);

	gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

layout(std140, binding = 4) uniform SceneData {
	mat4 u_VP;
	vec3 u_CameraPos;
	float padding;
};

layout(std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	ivec4 u_EntityData;
};

layout(std140, binding = 3) uniform MaterialData {
	vec4  u_AlbedoColor;
	float u_Metallic;
	float u_Roughness;
	float u_AO;
	float u_Emission;
	int   u_HasAlbedoMap;
	int   u_HasNormalMap;
	int   u_HasMetallicMap;
	int   u_HasRoughnessMap;
	int   u_HasMRMap;
	int   u_HasAOMap;
	vec2  u_Padding;
};

struct Light {
	vec4 Position;
	vec4 Color;
};

layout(std140, binding = 2) uniform LightData {
	int u_LightCount;
	Light u_Lights[16];
};

layout(binding = 0) uniform sampler2D u_AlbedoMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_MetallicMap;
layout(binding = 3) uniform sampler2D u_RoughnessMap;
layout(binding = 4) uniform sampler2D u_MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D u_AOMap;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	return a2 / max(PI * denom * denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return NdotV / max(NdotV * (1.0 - k) + k, 0.0001);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggxV = GeometrySchlickGGX(NdotV, roughness);
	float ggxL = GeometrySchlickGGX(NdotL, roughness);
	return ggxV * ggxL;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
	vec3 albedo = u_AlbedoColor.rgb;
	if (u_HasAlbedoMap == 1){
		//albedo *= texture(u_AlbedoMap, v_TexCoord).rgb;
		albedo *= pow(texture(u_AlbedoMap, v_TexCoord).rgb, vec3(2.2));
	}
		

	float metallic = u_Metallic;
	float roughness = u_Roughness;
	float ao = u_AO;

	if (u_HasMRMap == 1)
	{
		vec3 mr = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
		metallic = mr.b;
		roughness = mr.g;
	}
	else
	{
		if (u_HasMetallicMap == 1)
			metallic = texture(u_MetallicMap, v_TexCoord).r;

		if (u_HasRoughnessMap == 1)
			roughness = texture(u_RoughnessMap, v_TexCoord).r;
	}

	if (u_HasAOMap == 1)
		ao = texture(u_AOMap, v_TexCoord).r;

	roughness = clamp(roughness, 0.04, 1.0);
	metallic = clamp(metallic, 0.0, 1.0);
	ao = clamp(ao, 0.0, 1.0);

	vec3 N = normalize(v_TBN[2]);
	
	if (u_HasNormalMap == 1)
	{
		vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
		N = normalize(v_TBN * tangentNormal);
	}

	vec3 V = normalize(u_CameraPos - v_WorldPos);
	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	vec3 Lo = vec3(0.0);
	int lightCount = min(u_LightCount, 16);

	for (int i = 0; i < lightCount; i++)
	{
		vec3 lightPos = u_Lights[i].Position.xyz;
		float intensity = u_Lights[i].Position.w;
		vec3 lightColor = u_Lights[i].Color.rgb;

		vec3 L = normalize(lightPos - v_WorldPos);
		vec3 H = normalize(V + L);

		float distance = length(lightPos - v_WorldPos);
		float attenuation = 1.0 / max(distance * distance, 0.0001);
		vec3 radiance = lightColor * intensity * attenuation;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo + albedo * u_Emission;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	o_Color = vec4(color, 1.0);
	//o_Color = vec4(Lo, 1.0);
	//o_Color = vec4(normalize(u_Lights[0].Position.xyz - v_WorldPos), 1.0);
	//o_Color=vec4(vec3(float(u_LightCount) / 4.0), 1.0);
	o_EntityID = u_EntityData.x;
}
