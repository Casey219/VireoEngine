#pragma once
#include "Material.h"
#include "Animation/AnimationData.h"
#include "Renderer/VertexArray.h"

namespace Vireo
{
	struct MeshVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class Mesh {
	public:
		Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices) {
			m_VertexArray = VertexArray::Create();

			auto vb = VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(MeshVertex));
			vb->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal"   },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float3, "a_Tangent"  }, 
				{ ShaderDataType::Float3, "a_Bitangent"} 
				});
			m_VertexArray->AddVertexBuffer(vb);

			auto ib = IndexBuffer::Create(indices.data(), indices.size());
			m_VertexArray->SetIndexBuffer(ib);
			m_IndexCount = indices.size();
			//m_VertexArray->Unbind();
		}

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return m_IndexCount; }

	private:
		Ref<VertexArray> m_VertexArray;
		uint32_t m_IndexCount;
	};
}
