#include"Render3DLayer.h"
#include "Renderer/Renderer3D.h"

namespace Vireo {
	Render3DLayer::Render3DLayer()
		: Layer("Render3DLayer"), m_EditorCamera(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
	{
	}
	void Render3DLayer::OnAttach()
	{
		
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);


		Renderer3D::Init();
		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		
	}
	void Render3DLayer::OnDetach()
	{
		
	}
	void Render3DLayer::OnUpdate(Timestep ts)
	{
		
		m_EditorCamera.OnUpdate(ts);

		// 准备渲染环境
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// 开启深度测试
		RenderCommand::EnableDepthTest();

		// 开始 3D 场景渲染（上传 Camera Uniform Buffer）
		Renderer3D::BeginScene(m_EditorCamera);

		// 提交绘制请求
		// 绘制一个带纹理的平铺地面
		glm::mat4 floorTransform = glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 0.0f })
			* glm::scale(glm::mat4(1.0f), { 10.0f, 0.1f, 10.0f });
		Renderer3D::DrawCube(floorTransform,m_Texture,5.0f,{ 0.8f, 0.8f, 0.8f, 1.0f });

		// 绘制一个红色的旋转立方体
		static float rotation = 0.0f;
		rotation += ts * 50.0f;
		glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.5f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 1.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.5f });

		//
		Renderer3D::DrawCube(cubeTransform,nullptr,1.0f,{ 1.0f, 0.3f, 0.3f, 1.0f });

		// 结束场景
		Renderer3D::EndScene();
		

	}
	void Render3DLayer::OnImGuiRender()
	{
		
	}
	void Render3DLayer::OnEvent(Event& e)
	{
		
	}
}
