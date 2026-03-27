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
    //mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
    //vec3 T = normalize(normalMatrix * a_Tangent);
    //vec3 B = normalize(normalMatrix * a_Bitangent);
    //vec3 N = normalize(normalMatrix * a_Normal);
    //v_TBN = mat3(T, B, N);
	mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
	vec3 N = normalize(normalMatrix * a_Normal);
	vec3 T = normalize(normalMatrix * a_Tangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

v_TBN = mat3(T, B, N);

    gl_Position = u_ViewProjection * worldPos;
}



#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in mat3 v_TBN;

// --- 材质数据 (std140 严格对齐) ---
layout(std140, binding = 3) uniform MaterialData {
    vec4  u_AlbedoColor;      // [0-15]
    float u_Metallic;         // [16-19]
    float u_Roughness;        // [20-23]
    float u_AO;               // [24-27]
    float u_Emission;         // [28-31]
    int   u_HasAlbedoMap;     // [32-35]
    int   u_HasNormalMap;     // [36-39]
    int   u_HasMRMap;         // [40-43] (glTF 混合贴图标记)
    int   u_HasAOMap;         // [44-47]
    float u_Padding[4];       // 补齐至更高位
};

// --- 多光源数据 ---
struct Light {
    vec4 Position;  // w 为强度 
    vec4 Color;     // w 暂时留空或作为半径
};

layout(std140, binding = 2) uniform LightData {
    int u_LightCount;       // 当前场景光源数量
    float _pad[3];
    Light u_Lights[16];     // 假设最多支持 16 个光源
};

layout(std140, binding = 0) uniform SceneData { 
    mat4 u_VP; 
    vec3 u_CameraPos; 
    float padding;
};

// 纹理绑定 
layout(binding = 0) uniform sampler2D u_AlbedoMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 4) uniform sampler2D u_MetallicRoughnessMap;
layout(binding = 5) uniform sampler2D u_AOMap;

const float PI = 3.14159265359;

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
    // 1. 属性提取与线性化 
    vec3 albedo = u_AlbedoColor.rgb;
    if (u_HasAlbedoMap == 1) albedo *= pow(texture(u_AlbedoMap, v_TexCoord).rgb, vec3(2.2));

    float metallic = u_Metallic;
    float roughness = u_Roughness;
    if (u_HasMRMap == 1) {
        vec3 mr = texture(u_MetallicRoughnessMap, v_TexCoord).rgb;
        //roughness *= mr.g; // glTF: Roughness 是 G 通道 
        metallic *= mr.b;  // glTF: Metallic 是 B 通道 
    }
    roughness = max(roughness, 0.05); // 防止除零 

    // 2. 法线计算 
    vec3 N = normalize(v_TBN[2]);
	/*
    if (u_HasNormalMap == 1) {
        N = texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0;
        N = normalize(v_TBN * N);
    }
	*/
    vec3 V = normalize(u_CameraPos - v_WorldPos);
    vec3 F0 = mix(vec3(0.04), albedo, metallic); // 基础反射率 

    // 3. 多光源循环计算 
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_LightCount; ++i) {
        vec3 L = normalize(u_Lights[i].Position.xyz - v_WorldPos);
        vec3 H = normalize(V + L);
        
        float dist = length(u_Lights[i].Position.xyz - v_WorldPos);
        float attenuation = 1.0 / (dist * dist); // 距离衰减 
        vec3 radiance = u_Lights[i].Color.rgb * u_Lights[i].Position.w * attenuation;

        // Cook-Torrance BRDF 
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic); // 能量守恒 

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // 4. 环境光与最终输出 
    float ao = u_AO;
	
    if (u_HasAOMap == 1) {
		vec3 aoMap = texture(u_AOMap, v_TexCoord).rgb;
		//ao *= aoMap.r; // 只使用 R 通道作为 AO 值
		o_Color = vec4(ao, 0.0, 0.0, 1.0);
	}
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo + (albedo * u_Emission);

    // HDR & Gamma 
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    o_Color = vec4(color, 1.0);
}
