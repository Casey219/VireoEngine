#include"Render3DLayer.h"
#include "Renderer/Renderer3D.h"
#include "Utils/AssetManager.h"

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
		
		
		auto meshShader = Shader::Create("assets/shaders/BlinnPhong.glsl");
		//auto meshShader = Shader::Create("assets/shaders/DefaultMesh.glsl");

		// 加载模型（Model 类内部会调用 Assimp 并创建多个 Submesh）
		//m_BackpackModel = std::make_shared<Model>("assets/models/Sphere/globe-sphere.mtl", meshShader);
		//m_BackpackModel = std::make_shared<Model>("assets/models/Cerberus_Gun/Cerberus_LP.FBX", meshShader);
		m_BackpackModel = std::make_shared<Model>("assets/models/DamagedHelmet/DamagedHelmet.gltf", meshShader);
		//m_BackpackModel = std::make_shared<Model>("assets/models/Hyrule_Shield/HShield.obj", meshShader);
		//m_BackpackModel = AssetManager::GetModel("assets/models/Hyrule_Shield/HShield.obj",meshShader);

		VIR_CORE_ASSERT(m_BackpackModel->GetSubmeshes().size() > 0, "Failed to load model or model has no submeshes!");
		
		VIR_INFO("Model loaded with {0} submeshes", m_BackpackModel->GetSubmeshes().size());
		
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
		//Renderer3D::BeginScene(m_EditorCamera);
		static float time = 0.0f;
		time += ts;
		glm::vec3 dynamicLightPos = { glm::sin(time) * 5.0f, 2.0f, glm::cos(time) * 5.0f };
		//Renderer3D::BeginScene(m_EditorCamera, dynamicLightPos);
		Renderer3D::BeginScene(m_EditorCamera);
		/*
		// 提交绘制请求
		// 绘制一个带纹理的平铺地面
		glm::mat4 floorTransform = glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 0.0f })
			* glm::scale(glm::mat4(1.0f), { 10.0f, 0.1f, 10.0f });
		Renderer3D::DrawCube(floorTransform,m_Texture,5.0f,{ 0.8f, 0.8f, 0.8f, 1.0f });
*/
		// 绘制一个红色的旋转立方体
		static float rotation = 0.0f;
		rotation += ts * 50.0f;
		glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.5f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 1.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.5f });

		//
		//Renderer3D::DrawCube(cubeTransform,nullptr,1.0f,{ 1.0f, 0.3f, 0.3f, 1.0f });
		
		for (auto& submesh : m_BackpackModel->GetSubmeshes())
		{
			Renderer3D::DrawMesh(submesh.MeshData, submesh.Mat,cubeTransform);
		}

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
