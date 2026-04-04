#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;


layout(std140, binding = 4) uniform SceneData {
    mat4 u_ViewProjection;
    vec3 u_CameraPos; 
	float padding; 
};

layout(std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	ivec4 u_EntityData; // x: EntityID, yzw: padding
};


layout(location = 0) out vec3 v_Normal;
layout(location = 1) out vec3 v_WorldPos;
layout(location = 2) out vec2 v_TexCoord;

void main()
{
    v_WorldPos = vec3(u_Transform * vec4(a_Position, 1.0));
    
    v_Normal = normalize(mat3(transpose(inverse(u_Transform))) * a_Normal);
    
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(v_WorldPos, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location=1) out int color2;

layout(location = 0) in vec3 v_Normal;
layout(location = 1) in vec3 v_WorldPos;
layout(location = 2) in vec2 v_TexCoord;

// 场景级全局数据
layout(std140, binding = 4) uniform SceneData {
    mat4 u_ViewProjection;
    vec3 u_CameraPos; 
	float padding; 
};

layout(std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	ivec4 u_EntityData; // x: EntityID, yzw: padding
};

struct Light {
    vec4 Position;  // w 为强度 
    vec4 Color;     // w 暂时留空或作为半径
};

layout(std140, binding = 2) uniform LightData {
    int u_LightCount;       // 当前场景光源数量
    Light u_Lights[16];     // 假设最多支持 16 个光源
};

/*
layout(std140, binding = 3) uniform TestData {
	vec4 testVec;
};*/

layout(binding = 0) uniform sampler2D u_AlbedoTexture;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 4) uniform sampler2D u_MRMap;

const float SHININESS = 32.0;      // 高光指数
const float AMBIENT_STRENGTH = 0.1; // 环境光强度

void main()
{
    vec3 albedo = texture(u_AlbedoTexture, v_TexCoord).rgb;
    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraPos - v_WorldPos); // 确保变量名对齐
    
    vec3 totalDiffuseSpecular = vec3(0.0);
    
    for(int i = 0; i < u_LightCount; ++i)
    {
        Light light = u_Lights[i];
        vec3 lightVec = light.Position.xyz - v_WorldPos;
        float dist = length(lightVec);
        vec3 L = normalize(lightVec);
        
        float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
        float intensity = light.Position.w;
        
        // Diffuse
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * light.Color.rgb * albedo;
        
        // Specular (Blinn-Phong)
        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), SHININESS);
        vec3 specular = spec * light.Color.rgb*0.1; // 或者乘一个材质的高光系数
        
        totalDiffuseSpecular += (specular+diffuse ) * attenuation * intensity;
    }
    
    vec3 ambient = AMBIENT_STRENGTH * albedo;
    color = vec4(ambient + totalDiffuseSpecular, 1.0);
	color2=u_EntityData.x;

}
