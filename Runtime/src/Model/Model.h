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
		Model(const std::string& path, const Ref<Shader>& shader) {
			LoadModel(path, shader);
		}

		struct Submesh {
			Ref<Mesh> MeshData;
			Ref<Material> Mat;
		};

		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

	private:
		void LoadModel(const std::string& path, const Ref<Shader>& shader);
		void ProcessNode(aiNode* node, const aiScene* scene, const Ref<Shader>& shader);
		Submesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const Ref<Shader>& shader);

		std::vector<Submesh> m_Submeshes;
		std::string m_Directory;
	};
}
