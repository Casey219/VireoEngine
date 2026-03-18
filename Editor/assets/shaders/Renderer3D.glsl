#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in float a_TexIndex;
layout(location = 5) in float a_TilingFactor;
layout(location = 6) in int a_EntityID;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
};

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out flat float v_TexIndex;
layout(location = 3) out vec3 v_Normal;
layout(location = 4) out flat float v_TilingFactor;
layout(location = 5) out flat int v_EntityID;

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_Normal = a_Normal;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityColor; // 对应渲染目标的第二个附件 (RED_INTEGER)

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in flat float v_TexIndex;
layout(location = 3) in vec3 v_Normal;
layout(location = 4) in flat float v_TilingFactor;
layout(location = 5) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
    // 应用 Tiling Factor
    vec4 texColor = v_Color * texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor);
    
    // 基础光照
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(v_Normal), lightDir), 0.3);
    
    FragColor = texColor * diff;
    EntityColor = v_EntityID; // 将 ID 写入特殊附件
}
