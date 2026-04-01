#pragma once
#include"Renderer/Texture.h"
#include"Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"


namespace Vireo {
	class Material {
	public:
		struct alignas(16) PBRParams {
			glm::vec4 AlbedoColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			float Metallic = 0.0f;
			float Roughness = 0.5f;
			float AO = 1.0f;
			float Emission = 0.0f;
			int HasAlbedoMap = 0;
			int HasNormalMap = 0;
			int HasMetallicMap = 0;
			int HasRoughnessMap = 0;
			int HasMRMap = 0;
			int HasAOMap = 0;
			glm::vec2 Padding = { 0.0f, 0.0f };
			//float Padding[2];
		};

		Material() {
			m_PbrUBO = UniformBuffer::Create(sizeof(PBRParams), 3); // Binding 3
		};

		void Bind() {

			m_Params.HasAlbedoMap = AlbedoMap ? 1 : 0;
			if (AlbedoMap) AlbedoMap->Bind(0);

			m_Params.HasNormalMap = NormalMap ? 1 : 0;
			if (NormalMap) NormalMap->Bind(1);

			m_Params.HasMRMap = MetallicRoughnessMap ? 1 : 0;
			if (MetallicRoughnessMap) {
				MetallicRoughnessMap->Bind(4);
				m_Params.HasMetallicMap = 0;
				m_Params.HasRoughnessMap = 0;
			}
			else {
				m_Params.HasMetallicMap = MetallicMap ? 1 : 0;
				if (MetallicMap) MetallicMap->Bind(2);

				m_Params.HasRoughnessMap = RoughnessMap ? 1 : 0;
				if (RoughnessMap) RoughnessMap->Bind(3);
			}

			m_Params.HasAOMap = AOMap ? 1 : 0;
			if (AOMap) AOMap->Bind(5);

			m_PbrUBO->SetData(&m_Params, sizeof(PBRParams)); // 更新 UBO 数据
			 
		}

		PBRParams& GetParams() { return m_Params; }
		void SetAlbedo(const Ref<Texture2D>& tex) { AlbedoMap = tex; }
		void SetNormal(const Ref<Texture2D>& tex) { NormalMap = tex; }
		void SetMetallic(const Ref<Texture2D>& tex) {
			MetallicMap = tex;
		}
		void SetRoughness(const Ref<Texture2D>& tex) { RoughnessMap = tex; }
		void SetMetallicRoughness(const Ref<Texture2D>& tex) { MetallicRoughnessMap = tex; }
		void SetAO(const Ref<Texture2D>& tex) { AOMap = tex; }

	public:
		Ref<Texture2D> AlbedoMap;
		Ref<Texture2D> NormalMap;
		Ref<Texture2D> MetallicMap;
		Ref<Texture2D> RoughnessMap;
		Ref<Texture2D> MetallicRoughnessMap;
		Ref<Texture2D> AOMap;

	private:
		
		PBRParams m_Params;
		Ref<UniformBuffer> m_PbrUBO;
	};
}
