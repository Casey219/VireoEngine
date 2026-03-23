#type vertex
#version 450 core
precision highp float;
precision highp int;
precision highp sampler2D;
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

precision highp float;
precision highp int;
precision highp sampler2D;


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
/*
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}*/

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    // 1. 强制保护粗糙度，绝对不能为 0
    roughness = clamp(roughness, 0.04, 1.0); 
    
    float a = roughness * roughness;
    float a2 = a * a;
    
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    // --- 调试用：手动检查 NaN ---
    // 如果 NdotH2 或 a2 是 NaN，isinf/isnan 函数可以检测（GLSL 4.0+）
    // 简单做法：如果值异常，强制设为 0
    if (NdotH2 > 1.0 || NdotH2 < 0.0) NdotH2 = 0.0; 
    if (a2 > 1.0 || a2 < 0.0) a2 = 0.0;

    // 2. 标准 GGX 分母计算
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    
    // 3. 防止分母为 0 (虽然理论上 denom >= 1.0，但防万一)
    if (denom <= 0.0001) return 0.0;

    float result = a2 / (PI * denom * denom);
    // 4. 最终结果检查：如果是 Inf 或 NaN，返回 0
    if (result > 10000.0 || result < 0.0) return 0.0;

    return result;
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

    // --- 1. 数据初始化 (统一作用域) ---
    vec3  albedo    = u_AlbedoColor.rgb;
    float metallic  = u_Metallic;
    float roughness = u_Roughness;
    float ao        = u_AO;
	//vec3 albedo=vec3(1.0,1.0,1.0);
	//float metallic = 0.0;
	//float roughness = 0.5;
	//float ao = 1.0;

    // --- 2. 贴图采样逻辑 ---
    
    // Albedo
    if (u_HasAlbedoMap == 1) {
		vec3 sampledAlbedo = texture(u_AlbedoMap, v_TexCoord).rgb;
        
        // 关键：检查采样结果是否合法
        if (any(isnan(sampledAlbedo)) || any(isinf(sampledAlbedo))) {
            sampledAlbedo = vec3(1.0, 0.0, 1.0); // 出错变洋红，方便调试
        }
        albedo = sampledAlbedo;
    }
    // Metallic & Roughness (处理 glTF 混合贴图与独立贴图)
	
    if (u_HasMRMap == 1) {
		vec3 mrTextureSample = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
        
        // glTF 标准: G = Roughness, B = Metallic
        float r_raw = mrTextureSample.g;
        float m_raw = mrTextureSample.b;

        //  防御：检查是否读出了 NaN (常见于损坏的纹理或驱动 Bug)
        if (isnan(r_raw)) r_raw = 0.5;
        if (isnan(m_raw)) m_raw = 0.0;

        //roughness = r_raw;
        //metallic = m_raw;
    } else {
        //if (u_HasMetallicMap == 1)  metallic  = texture(u_MetallicMap, v_TexCoord).r;
        //if (u_HasRoughnessMap == 1) roughness = texture(u_RoughnessMap, v_TexCoord).r;
    }
	

    // Ambient Occlusion
    // 注意：如果 MR 贴图的 R 通道是黑的，u_HasAOMap 应该在 C++ 里设为 0
    if (u_HasAOMap == 1) {
        //ao = texture(u_AOMap, v_TexCoord).r;
    }
	//ao=texture(u_AOMap, v_TexCoord).r;
	//o_Color = vec4(ao, 0.0, 0.0, 1.0);

    // 数学安全保护
    roughness = clamp(roughness, 0.05, 1.0);
    metallic  = clamp(metallic, 0.00, 1.0);
	
    // --- 3. PBR 光照计算 (基于重构后的变量) ---
    
    // 法线
	/*
    vec3 N;
    if (u_HasNormalMap == 1) {
        N = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
        N = normalize(v_TBN * N);
    } else {
        N = normalize(v_TBN[2]);
    }*/

	vec3 N;
	N = normalize(v_TBN[2]); 
    
    // 再次检查 N 是否有效
    if (any(isnan(N)) || length(N) < 0.001) {
        N = vec3(0.0, 0.0, 1.0);
    }

    vec3 V = normalize(u_CameraPos - v_WorldPos);
    vec3 L = normalize(u_LightPos.xyz - v_WorldPos);
    vec3 H = normalize(V + L);

    // 基础反射率 F0
    vec3 F0 = vec3(0.04); 
	
    F0 = mix(F0, albedo, metallic);
	//F0 = mix(vec3(0.04), albedo, metallic);
	//F0=(1.0-metallic)*vec3(0.04)+metallic*vec3(albedo);
	//F0=metallic*vec3(0.04);
	//o_Color = vec4(F0, 1.0);
	
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);

    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);
	//o_Color = vec4(F, 1.0);
	//o_Color = vec4(NDF, G,0.0, 1.0);
	//o_Color = vec4(NDF, G, F.r, 1.0);
    vec3  numerator   = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3  specular    = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    vec3  radiance = u_LightColor.rgb * u_LightColor.a; // 使用 alpha 作为强度
    vec3  Lo = (kD * albedo / PI + specular) * radiance * NdotL;
	//o_Color = vec4(kD * albedo / PI, 1.0);
	//o_Color = vec4(specular, 1.0);


	//ao=clamp(ao,0.0,1.0);
    // --- 4. 最终输出 ---
    vec3 ambient = vec3(0.03) * albedo * ao;

	
	vec3 ambientWithAO = ao*ambient;
	
    vec3 color = ambient + Lo + (albedo * u_Emission);
    // HDR & Gamma
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

	o_Color = vec4(color, 1.0);
	
}
