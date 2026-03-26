#pragma once
#include <Camera/Camera.h>
#include <Camera/EditorCamera.h>
#include "Texture.h"
#include <Model/Material.h>
#include <Model/Mesh.h>

namespace Vireo {
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const EditorCamera& camera);
		//static void BeginScene(const EditorCamera& camera, const glm::vec3& lightPos=glm::vec3(0.0,0.0,1.0), const glm::vec4& lightColor = glm::vec4(1.0));
		static void EndScene();

		static void DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture,float tilingFactor=1.0,const glm::vec4& color= glm::vec4(1.0f), int entityID=-1);
		// Renderer3D.h
		static void DrawMesh(const Ref<Mesh>& mesh, const Ref<Material>& material, const Ref<Shader>& shader, const glm::mat4& transform= glm::mat4(1.0f), int entityID = -1);
		static void Flush();
		static void NextBatch();
		static void StartBatch();
		
	};

	struct alignas(16) Light {
		glm::vec4 Position;  // [x, y, z, Intensity] -> w分量存储强度
		glm::vec4 Color;     // [r, g, b, _unused]
	};

	struct LightSceneData {
		int LightCount = 0;
		float Padding[3];    // 补齐到 16 字节对齐
		Light Lights[16];    // 必须与 Shader 中的数组大小一致
	};
}
