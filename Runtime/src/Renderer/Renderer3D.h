#pragma once
#include <Camera/Camera.h>
#include <Camera/EditorCamera.h>
#include "Texture.h"

namespace Vireo {
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void DrawCube(const glm::mat4& transform, const Ref<Texture2D>& texture,float tilingFactor=1.0,const glm::vec4& color= glm::vec4(1.0f), int entityID=-1);
		static void Flush();
		static void NextBatch();
		static void StartBatch();
		
	};
}
