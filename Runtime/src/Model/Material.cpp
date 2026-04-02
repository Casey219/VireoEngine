#include"virpch.h"
#include"Material.h"
#include "Utils/AssetManager.h"

namespace Vireo {

	Material::Material()
	{
		m_PbrUBO = UniformBuffer::Create(sizeof(PBRParams), 3);
	}

	void Material::Bind()
	{
		m_Params.HasAlbedoMap = AlbedoMap ? 1 : 0;
		(AlbedoMap ? AlbedoMap : AssetManager::GetWhiteTexture())->Bind(0);

		m_Params.HasNormalMap = NormalMap ? 1 : 0;
		(NormalMap ? NormalMap : AssetManager::GetFlatNormalTexture())->Bind(1);

		m_Params.HasMRMap = MetallicRoughnessMap ? 1 : 0;
		(MetallicRoughnessMap ? MetallicRoughnessMap : AssetManager::GetDefaultMRTexture())->Bind(4);

		if (MetallicRoughnessMap)
		{
			m_Params.HasMetallicMap = 0;
			m_Params.HasRoughnessMap = 0;

			AssetManager::GetBlackTexture()->Bind(2);
			AssetManager::GetWhiteTexture()->Bind(3);
		}
		else
		{
			m_Params.HasMetallicMap = MetallicMap ? 1 : 0;
			(MetallicMap ? MetallicMap : AssetManager::GetBlackTexture())->Bind(2);

			m_Params.HasRoughnessMap = RoughnessMap ? 1 : 0;
			(RoughnessMap ? RoughnessMap : AssetManager::GetWhiteTexture())->Bind(3);
		}

		m_Params.HasAOMap = AOMap ? 1 : 0;
		(AOMap ? AOMap : AssetManager::GetWhiteTexture())->Bind(5);

		m_PbrUBO->SetData(&m_Params, sizeof(PBRParams));

		/*VIR_CORE_INFO(
			"Material Bind -> HasMRMap={0}, HasMetallicMap={1}, HasRoughnessMap={2}, HasAOMap={3}, Metallic={4}, Roughness={5}, AO={6}",
			m_Params.HasMRMap,
			m_Params.HasMetallicMap,
			m_Params.HasRoughnessMap,
			m_Params.HasAOMap,
			m_Params.Metallic,
			m_Params.Roughness,
			m_Params.AO
		);*/
	}
}
