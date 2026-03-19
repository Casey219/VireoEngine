#include "virpch.h"
#include "Model.h"
#include<assimp/Importer.hpp>
#include <assimp/postprocess.h>
namespace Vireo {
	void Model::LoadModel(const std::string& path, const Ref<Shader>& shader) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) return;
		m_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene, shader);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, const Ref<Shader>& shader) {
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Submeshes.push_back(ProcessMesh(mesh, scene, shader));
		}
		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, shader);
		}
	}

	Model::Submesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const Ref<Shader>& shader) {
		// 1. 提取顶点数据
		std::vector<MeshVertex> vertices;
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			MeshVertex v;
			v.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			v.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
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
		Ref<Material> material = std::make_shared<Material>(shader);
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
			aiString str;
			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS|| aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &str) == AI_SUCCESS) {
				std::string texPath = m_Directory + "/" + str.C_Str();
				material->SetAlbedo(Texture2D::Create(texPath));
				VIR_CORE_INFO("Loaded texture: {0}", texPath);
			}
			else {
				VIR_CORE_WARN("Mesh has material but no diffuse texture found!");
				Ref<Texture2D> WhiteTexture = Texture2D::Create(1, 1);
				uint32_t whiteData = 0xffffffff;
				WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
				material->SetAlbedo(WhiteTexture);
			}
		}

		return { std::make_shared<Mesh>(vertices, indices), material };
	}
}
