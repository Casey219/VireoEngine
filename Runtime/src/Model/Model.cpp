#include "virpch.h"
#include "Model.h"
#include<assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Utils/AssetManager.h>
#include <filesystem>
namespace Vireo {
	void Model::LoadModel(const std::string& path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |
			aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return;

		m_Directory = std::filesystem::path(path).parent_path().generic_string();

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene) {
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Submeshes.push_back(ProcessMesh(mesh, scene));
		}
		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Model::Submesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		// 1. 提取顶点数据
		std::vector<MeshVertex> vertices;
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			MeshVertex v;
			v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			v.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			if (mesh->mTangents)
				v.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			if (mesh->mTextureCoords[0])
				v.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			else
				v.TexCoord = { 0.0f, 0.0f };
			vertices.push_back(v);
		}

		// 2. 提取索引
		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		// 3. 处理材质联系 
		Ref<Material> material = std::make_shared<Material>();
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
			aiString str;
			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &str) == AI_SUCCESS) {
				std::string texPath = (std::filesystem::path(m_Directory) / str.C_Str()).generic_string();
				material->SetAlbedo(AssetManager::GetTexture(texPath));
			}
			else {
				VIR_CORE_WARN("Mesh has material but no diffuse texture found!");
				
				material->SetAlbedo(AssetManager::GetWhiteTexture());
			}

			if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_HEIGHT, 0, &str) == AI_SUCCESS) {
				material->NormalMap = AssetManager::GetTexture(m_Directory + "/" + str.C_Str());
				VIR_CORE_INFO("Loaded normal map: {0}", m_Directory + "/" + str.C_Str());
			}
			if (aiMat->GetTexture(aiTextureType_METALNESS, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_SPECULAR, 0, &str) == AI_SUCCESS)
			{
				material->MetallicMap = AssetManager::GetTexture(m_Directory + "/" + str.C_Str());
				VIR_CORE_INFO("Loaded metallic map: {0}", m_Directory + "/" + str.C_Str());
			}
			if(aiMat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &str) == AI_SUCCESS)
			{
				material->RoughnessMap = AssetManager::GetTexture(m_Directory + "/" + str.C_Str());
				VIR_CORE_INFO("Loaded roughness map: {0}", m_Directory + "/" + str.C_Str());
			}
			if(aiMat->GetTexture(aiTextureType_UNKNOWN, 0, &str) == AI_SUCCESS)
			{
				material->MetallicRoughnessMap = AssetManager::GetTexture(m_Directory + "/" + str.C_Str());
				VIR_CORE_INFO("Loaded metallic-roughness map: {0}", m_Directory + "/" + str.C_Str());
			}
			if(aiMat->GetTexture(aiTextureType_AMBIENT, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &str) == AI_SUCCESS)
			{
				material->AOMap = AssetManager::GetTexture(m_Directory + "/" + str.C_Str());
				VIR_CORE_INFO("Loaded AO map: {0}", m_Directory + "/" + str.C_Str());
			}
				
		}

		return { std::make_shared<Mesh>(vertices, indices), material};
	}
}
