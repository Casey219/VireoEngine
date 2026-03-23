#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    vec3 u_CameraPos;
	float padding;
};

layout(std140, binding = 1) uniform ModelData {
    mat4 u_Transform;
    int u_EntityID;
	float v_Padding[3];
};

layout(location = 0) out vec3 v_WorldPos;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out mat3 v_TBN;

void main() {
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;
    v_TexCoord = a_TexCoord;

    // 法线矩阵，处理非统一缩放
    mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 B = normalize(normalMatrix * a_Bitangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    v_TBN = mat3(T, B, N);

    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

// --- 材质数据 UBO ---
// 必须与 Material.h 中的 PBRParams 严格对齐 (std140: 16字节对齐)
layout(std140, binding = 3) uniform MaterialData {
    vec4  u_AlbedoColor;      // 0  - 16 bytes
    float u_Metallic;         // 16 - 4  bytes
    float u_Roughness;        // 20 - 4  bytes
    float u_AO;               // 24 - 4  bytes
    float u_Emission;         // 28 - 4  bytes
    
    int u_HasAlbedoMap;       // 32 - 4  bytes
    int u_HasNormalMap;       // 36 - 4  bytes
    int u_HasMetallicMap;     // 40 - 4  bytes
    int u_HasRoughnessMap;    // 44 - 4  bytes
    int u_HasMRMap;           // 48 - 4  bytes
    int u_HasAOMap;           // 52 - 4  bytes
    float u_Padding[2];       // 56 - 8  bytes (补齐到 64 字节)
};

layout(binding = 0) uniform sampler2D u_AlbedoMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_MetallicMap;
layout(binding = 3) uniform sampler2D u_RoughnessMap;
layout(binding = 4) uniform sampler2D u_MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D u_AOMap;

layout(std140, binding = 0) uniform SceneData { 
    mat4 u_VP; 
    vec3 u_CameraPos; 
	float padding;
};

layout(std140, binding = 2) uniform LightData { 
    vec4 u_LightPos; 
    vec4 u_LightColor; 
};

const float PI = 3.14159265359;

// --- PBR 数学函数 ---
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0; 
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, k) * GeometrySchlickGGX(NdotL, k);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    // 1. 属性提取
    vec3 albedo = u_AlbedoColor.rgb;
	//o_Color=vec4(albedo,1.0);
    if (u_HasAlbedoMap == 1) albedo *= texture(u_AlbedoMap, v_TexCoord).rgb;
	//o_Color=vec4(albedo,1.0);

    float metallic = u_Metallic;
    float roughness = u_Roughness;
	//o_Color=vec4(vec3(roughness),1.0);
    // 独立贴图采样

	//o_Color = vec4(roughness, 0.0, 0.0, 1.0);
    // 合并贴图采样 (glTF 标准: G=Roughness, B=Metallic)
    if (u_HasMRMap == 1) {
        vec3 mr = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
        //roughness *= mr.g;
		//metallic *= mr.b;
		//roughness = mr.g;
        //metallic = mr.b;
		roughness=texture(u_MetallicRoughnessMap, v_TexCoord).g;
		metallic = texture(u_MetallicRoughnessMap, v_TexCoord).b;
        //o_Color = vec4(roughness, 0.0, 0.0, 1.0);
    }
	else{
		if (u_HasMetallicMap == 1) metallic *= texture(u_MetallicMap, v_TexCoord).r;
		if (u_HasRoughnessMap == 1) roughness *= texture(u_RoughnessMap, v_TexCoord).r;
	}
	vec3 mr = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
	vec3 testMR = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
float testMetallic =testMR.b;
//testMetallic*=metallic;
float testRoughness = testMR.g;

// 如果输出这个还是黑的，说明你的 Binding 4 根本没绑上贴图
// 如果输出这个是红色的，说明是上面的 if (u_HasMRMap == 1) 没进去
//o_Color = vec4(testMetallic, 0.0, 0.0, 1.0);
//o_Color = vec4(roughness, 0.0, 0.0, 1.0);
	//o_Color=vec4(mr,1.0);
	//o_Color=vec4(mr.g,0.0,0.0,1.0);
	//o_Color= vec4(metallic,0.0,0.0,1.0);
	//o_Color= vec4(vec3(metallic),1.0);
	//o_Color=vec4(0.0,roughness,metallic,1.0);
	//o_Color= vec4(vec3(roughness),1.0);
	//o_Color=vec4(texture(u_MetallicRoughnessMap, v_TexCoord).rgb,1.0);
	//o_Color=vec4(roughness,0.0,0.0,1.0);
	//o_Color=vec4(u_HasMRMap, u_HasMetallicMap, u_HasRoughnessMap, 1.0);

    float ao = u_AO;
   // if (u_HasAOMap == 1) ao *= texture(u_AOMap, v_TexCoord).r;
	//o_Color=vec4(ao,0.0,0.0,1.0);
    // 防止粗糙度过低导致黑点
    roughness = max(roughness, 0.05);

    // 2. 法线计算 (TBN 空间)
    vec3 N;
    if (u_HasNormalMap == 1) {
        N = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
        N = normalize(v_TBN * N);
    } else {
        N = normalize(v_TBN[2]);
    }

    // 3. 直接光照计算 (Cook-Torrance BRDF)
    vec3 V = normalize(u_CameraPos - v_WorldPos);
    vec3 L = normalize(u_LightPos.xyz - v_WorldPos);
    vec3 H = normalize(V + L);

    // 基础反射率
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; 
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    
	vec3 radiance = u_LightColor.rgb * u_LightPos.w; // w 可以作为强度控制
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    // 4. 最终颜色输出
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo + (albedo * u_Emission);



    // HDR Tonemapping & Gamma Correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    o_Color = vec4(color, 1.0);
	//ao=1.0;
	//o_Color= vec4(metallic,0.0,0.0,1.0);
	//o_Color=vec4(albedo * ao,1.0);
	//o_Color=vec4(Lo,1.0);
	//o_Color=vec4(albedo,1.0);
	//o_Color=vec4(ambient,1.0);

	//kD = (vec3(1.0) - kS);
	//o_Color=vec4(kD,1.0);
	//o_Color=vec4(kD * albedo / PI + specular,1.0);

	//vec3 mr = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
	//o_Color=vec4(mr.b,0.0,0.0,1.0);

	
}
