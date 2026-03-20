#pragma once
#include"Renderer/Texture.h"
#include"Renderer/Shader.h"



namespace Vireo {
	class Material {
	public:
		Material() {
				
		}

		void Bind() {
			if (m_AlbedoTexture) m_AlbedoTexture->Bind(0);
		}

		void SetAlbedo(const Ref<Texture2D>& texture) { m_AlbedoTexture = texture; }

	private:
		Ref<Texture2D> m_AlbedoTexture;
		glm::vec4 m_Color{ 1.0f };
		float m_TilingFactor = 1.0f;
	};
}
