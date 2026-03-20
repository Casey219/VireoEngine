#pragma once
#include <assimp/material.h>
#include <assimp/matrix4x4.h>
#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>



namespace Vireo
{
	class Model {
	public:
		Model(const std::string&path):m_Path(path) {
			LoadModel(path);
		}

		struct Submesh {
			Ref<Mesh> MeshData;
			Ref<Material> MaterialData;
		};

		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }
		std::string GetPath() const { return m_Path; }

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Submesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Submesh> m_Submeshes;
		std::string m_Directory;
		std::string m_Path;
	};
}
