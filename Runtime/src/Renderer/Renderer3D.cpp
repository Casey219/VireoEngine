#include "virpch.h"
#include "Renderer3D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

namespace Vireo {
	
	struct CubeVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor; 

		//for editor
		int EntityID;       
	};

	struct MeshElementData {
		glm::mat4 Transform;
		int EntityID;
		float Padding[3]; // 补齐到 16 字节对齐，确保 std140 布局正确
	};

	struct SceneData {
		glm::mat4 ViewProjection; // 64 bytes
		glm::vec3 CameraPosition; // 12 bytes
		float Padding;            // 4 bytes (对齐到 16 字节)
	};

	struct LightData {
		glm::vec4 Position;       // 16 bytes (w 可以存强度 Intensity)
		glm::vec4 Color;          // 16 bytes
	};

	struct Renderer3DData {
		static const uint32_t MaxCubes = 10000;
		static const uint32_t MaxVertices = MaxCubes * 24; // 每个立方体 24 个独立顶点
		static const uint32_t MaxIndices = MaxCubes * 36;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> CubeShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White

		// 标准单位立方体顶点模板 (24顶点)
		struct CubeFaceData {
			glm::vec3 Pos;
			glm::vec3 Normal;
			glm::vec2 UV;
		};
		std::vector<CubeFaceData> UnitCube;

		SceneData SceneBuffer;
		Ref<UniformBuffer> SceneUBO;

		LightData LightBuffer;
		Ref<UniformBuffer> LightUBO;

		Ref<UniformBuffer> MeshUniformBuffer;
		MeshElementData MeshBuffer;
	};
	static Renderer3DData s_Data;

	void Renderer3D::Init() {
		// 1. 初始化顶点数组与缓冲
		s_Data.CubeVertexArray = VertexArray::Create();
		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"   },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
		});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

		// 2. 生成索引 (每 4 个顶点对应一个面 6 个索引)
		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0; indices[i + 1] = offset + 1; indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2; indices[i + 4] = offset + 3; indices[i + 5] = offset + 0;
			offset += 4;
		}
		s_Data.CubeVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, s_Data.MaxIndices));
		delete[] indices;

		// 3. 基础资源配置
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.CubeShader = Shader::Create("assets/shaders/Renderer3D.glsl");
		s_Data.SceneUBO = UniformBuffer::Create(sizeof(SceneData), 0);

		// 4. 定义单位立方体 (此处仅示例一个面的逻辑，实际需填满 6 个面)
		// 每个面 4 个点: {Pos, Normal, UV}
		// Front Face (Z+)
		s_Data.UnitCube = {
			// 前面 (Front Face) - Z+
			{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},

			// 后面 (Back Face) - Z-
			{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
			{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},

			// 左面 (Left Face) - X-
			{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

			// 右面 (Right Face) - X+
			{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
			{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

			// 顶面 (Top Face) - Y+
			{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},

			// 底面 (Bottom Face) - Y-
			{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}
		};


		s_Data.MeshUniformBuffer = UniformBuffer::Create(sizeof(MeshElementData), 1);
		s_Data.LightUBO = UniformBuffer::Create(sizeof(LightData), 2);
	}

	void Renderer3D::BeginScene(const EditorCamera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) {
		// 1. 更新相机数据 (Binding 0)
		s_Data.SceneBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.SceneBuffer.CameraPosition = camera.GetPosition();
		s_Data.SceneUBO->SetData(&s_Data.SceneBuffer, sizeof(SceneData));

		// 2. 更新灯光数据 (Binding 2)
		s_Data.LightBuffer.Position = glm::vec4(lightPos, 1.0f);
		s_Data.LightBuffer.Color = lightColor;
		s_Data.LightUBO->SetData(&s_Data.LightBuffer, sizeof(LightData));

		StartBatch();
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor,const glm::vec4& color, int entityID) {
		if (s_Data.CubeIndexCount >= s_Data.MaxIndices) NextBatch();

		float texIndex = 0.0f;
		if (texture) {
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
				if (*s_Data.TextureSlots[i] == *texture) { texIndex = (float)i; break; }
			}
			if (texIndex == 0.0f) {
				texIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
			}
		}

		// 遍历立方体的 24 个顶点
		for (const auto& unitVertex : s_Data.UnitCube) {
			s_Data.CubeVertexBufferPtr->Position = transform * glm::vec4(unitVertex.Pos, 1.0f);
			s_Data.CubeVertexBufferPtr->Normal = glm::mat3(transform) * unitVertex.Normal; // 法线需随旋转变换
			s_Data.CubeVertexBufferPtr->Color = color;
			s_Data.CubeVertexBufferPtr->TexCoord = unitVertex.UV;
			s_Data.CubeVertexBufferPtr->TexIndex = texIndex;
			s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor; // 赋值
			s_Data.CubeVertexBufferPtr->EntityID = entityID;         // 赋值
			s_Data.CubeVertexBufferPtr++;
		}
		s_Data.CubeIndexCount += 36;
	}

	// Renderer3D.cpp
	void Renderer3D::DrawMesh(const Ref<Mesh>& mesh, const Ref<Material>& material, const Ref<Shader>& shader,const glm::mat4& transform, int entityID)
	{
		s_Data.MeshBuffer.Transform = transform;
		s_Data.MeshBuffer.EntityID = entityID;
		s_Data.MeshUniformBuffer->SetData(&s_Data.MeshBuffer, sizeof(MeshElementData));

		// 2. 绑定材质 
		material->Bind();
		shader->Bind();
		// 3. 绑定几何并绘制
		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray(), mesh->GetIndexCount());

		// 4. 清理
		mesh->GetVertexArray()->Unbind();
	}

	void Renderer3D::Flush() {
		if (s_Data.CubeIndexCount == 0) return;
		uint32_t size = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
		s_Data.CubeVertexArray->Bind();
		s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, size);
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) s_Data.TextureSlots[i]->Bind(i);
		s_Data.CubeShader->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
		//s_Data.CubeShader->Unbind();
		s_Data.CubeVertexArray->Unbind();
	}

	void Renderer3D::StartBatch() {
		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::NextBatch() { Flush(); StartBatch(); }
	void Renderer3D::EndScene() { Flush(); }

}
