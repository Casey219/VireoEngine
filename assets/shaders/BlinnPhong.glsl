#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;


layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    vec3 u_CameraPosition; 
	float padding; 
};

layout(std140, binding = 1) uniform ModelData {
    mat4 u_Transform;
	int u_EntityID; 
	float v_Padding[3];
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

layout(location = 0) in vec3 v_Normal;
layout(location = 1) in vec3 v_WorldPos;
layout(location = 2) in vec2 v_TexCoord;

// 场景级全局数据
layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    vec3 u_CameraPos; 
	float padding; 
};

// 这里可以新开一个 Binding 2 专门放灯光，或者整合进 SceneData
layout(std140, binding = 2) uniform LightData {
    vec4 u_LightPos;   
    vec4 u_LightColor; 
};

layout(binding = 0) uniform sampler2D u_AlbedoTexture;

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos.xyz - v_WorldPos);
    vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
    vec3 texColor = texture(u_AlbedoTexture, v_TexCoord).rgb;

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor.rgb;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor.rgb;

    // Specular 
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); // 32.0 是高光指数(Shininess)
    vec3 specular = spec * u_LightColor.rgb;

    // 
    vec3 result = (ambient + diffuse + specular) * texColor;
    color = vec4(result, 1.0);
	//color = vec4(norm * 0.5 + 0.5, 1.0);
}
