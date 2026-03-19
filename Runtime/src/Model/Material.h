#pragma once
#include"Renderer/Texture.h"
#include"Renderer/Shader.h"



namespace Vireo {
	class Material {
	public:
		Material(const Ref<Shader>& shader) : m_Shader(shader) {}

		void Bind() {
			m_Shader->Bind();
			if (m_AlbedoTexture) m_AlbedoTexture->Bind(0);
			/*m_Shader->SetFloat4("u_Material.Color", m_Color);
			m_Shader->SetFloat("u_Material.TilingFactor", m_TilingFactor);*/
		}

		void SetAlbedo(const Ref<Texture2D>& texture) { m_AlbedoTexture = texture; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		Ref<Shader> GetShader() const { return m_Shader; }

	private:
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_AlbedoTexture;
		glm::vec4 m_Color{ 1.0f };
		float m_TilingFactor = 1.0f;
	};
}
