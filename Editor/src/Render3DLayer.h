#pragma once
#include"Vireo.h"
#include <Renderer/UniformBuffer.h>
#include "Model/Model.h"

namespace Vireo {
	class Render3DLayer : public Layer
	{
	public:
		Render3DLayer();
		virtual ~Render3DLayer()=default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		
		Ref<Framebuffer> m_Framebuffer;
		EditorCamera m_EditorCamera;
		Ref<VertexArray> m_CubeVA;
		Ref<Shader> m_CubeShader;
		uint32_t m_CubeIndexCount;

		struct MVPMatrix {
			glm::mat4 u_Transform;
			glm::mat4 u_ViewProjection;
		};

		MVPMatrix m_MVPMatrix;

		Ref<UniformBuffer> m_UniformBuffer;
		Ref<Texture2D> m_Texture;

		Ref<Model> m_BackpackModel;
		Ref<Shader> m_MeshShader;

		Ref<Scene> m_Scene;
	};
}
