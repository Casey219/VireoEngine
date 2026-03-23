// Mesh.glsl
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform CameraData
{
    mat4 u_ViewProjection;
};

layout(std140, binding = 1) uniform RendererData
{
    mat4 u_Transform;
    int u_EntityID;
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Normal;

void main()
{
    v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;

layout(binding = 0) uniform sampler2D u_AlbedoTexture;

void main()
{
    vec4 texColor = texture(u_AlbedoTexture, v_TexCoord);
	//vec4 texColor = vec4(1.0,0.0,0.0,1.0);
	// »ù´¡¹âÕÕ
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(v_Normal), lightDir), 0.3);
    
    //FragColor = texColor * diff;
	FragColor = vec4(texColor.rgb * diff, texColor.a);
	//FragColor=vec4(1.0, 0.0, 0.0, 1.0);
}
